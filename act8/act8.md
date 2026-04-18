# Activity 8: Memory Management and Paging

## ภาพรวมของ Activity 8
Activity 8 เป็นการทดลองเรื่องการจัดการหน่วยความจำ (memory management) โดยเน้นที่เทคนิคการแบ่งหน้า (paging) เพื่อแปลงที่อยู่เสมือนไปเป็นที่อยู่กายภาพ

ส่วนสำคัญในกิจกรรมนี้คือ:
- การจำลองตารางหน้าแบบหนึ่งระดับ (`paging_1level.c`)
- การจำลองตารางหน้าแบบสองระดับ (`paging_2level.c`)
- การใช้ page table เพื่อเก็บข้อมูลว่า page ใดอยู่ใน frame ไหน

## `paging_1level.c`

### แนวคิดหลัก
โค้ดนี้จำลองระบบที่อยู่เสมือน 16 บิต โดยแบ่งเป็น:
- page number 8 บิต
- offset 8 บิต

หมายความว่า logical address มีขนาด 16 บิต และ page size เท่ากับ 256 ไบต์

### องค์ประกอบสำคัญ
- `PageTableEntry` มี field:
  - `present` 1 บิต: ระบุว่า page นั้นถูกแมปอยู่หรือยัง
  - `frame` 15 บิต: เลข frame กายภาพที่ page ถูกแมปไป
- `page_table[PAGE_ENTRIES]` เป็นตารางหน้าแบบตรง ๆ (single-level)
- `physical_memory` เป็นบัฟเฟอร์จำลองหน่วยความจำกายภาพ

### การทำงานของฟังก์ชันหลัก
- `translate_address(logical_address)`:
  - แยก `page_number` จาก 8 บิตบนของ address
  - ถ้า page ยังไม่ present จะสุ่มเลือก frame ใหม่และตั้งค่าใน page table
  - คำนวณ physical address = frame<<8 + offset
  - แสดงผลการแปลงที่อยู่
- `read_from_memory` และ `write_to_memory`:
  - เรียก `translate_address` เพื่อแปลง address
  - อ่านหรือเขียนค่าจากหน่วยความจำกายภาพ

### flow ใน `main`
- จอง physical memory ขนาด 256 * 256 ไบต์
- เขียนและอ่านค่าที่ address ต่าง ๆ เช่น `0x123`, `0x1234`, `0xFF12`
- พิมพ์สถานะ page table ทั้งหมด
- คำนวณขนาดของ page table

### ข้อสังเกต
- โค้ดนี้ยังใช้การกำหนด frame แบบง่าย ๆ ด้วย `rand()` โดยไม่จัดการ page replacement
- page table มีขนาดคงที่และแมปโดยตรงจาก page number ไปยัง frame

## `paging_2level.c`

### แนวคิดหลัก
โค้ดนี้จำลองตารางหน้าแบบสองระดับ เพื่อใช้หน่วยความจำสำหรับตารางหน้าให้มีประสิทธิภาพมากขึ้นเมื่อที่อยู่เสมือนมีขนาดใหญ่

โครงสร้างที่แบ่งได้คือ:
- outer page number: 4 บิต
- inner page number: 4 บิต
- offset: 8 บิต

นี้หมายถึง logical address 16 บิตถูกแบ่งเป็น 3 ส่วน: outer page, inner page, offset

### องค์ประกอบสำคัญ
- `outer_page_table` เป็น array ของ pointer ที่ชี้ไปยัง inner page table
- inner page table แต่ละชุดเป็น `PageTableEntry[PAGE_ENTRIES]`
- `frame_allocated` ใช้ติดตามว่ากรอบหน่วยความจำกายภาพไหนถูกใช้งานแล้ว

### การทำงานของฟังก์ชันหลัก
- `translate_address(logical_address)`:
  - แยก `outer_page_number`, `page_number`, และ `offset`
  - ถ้า inner page table สำหรับ outer entry ยังไม่ถูกสร้าง จะสร้างด้วย `calloc`
  - ถ้า page ยังไม่ present จะหา frame ว่างจาก `frame_allocated` แล้วแมปไปยัง frame นั้น
  - คำนวณ physical address จาก frame และ offset
  - พิมพ์ผลการแปลงที่อยู่
- `read_from_memory` และ `write_to_memory` ทำงานเหมือนเลเวลเดียว แต่ใช้ translate ที่ซับซ้อนขึ้น

### flow ใน `main`
- เรียก `srand(time(NULL))`
- จอง physical memory เป็นบัฟเฟอร์
- เขียนและอ่านค่าจาก address เช่น `0x123`, `0x1234`, `0xFF12`
- พิมพ์สถานะ outer page table และ inner page table ที่ถูกจัดสรรแล้ว
- คำนวณขนาดของ outer page table และ inner page tables ที่สร้างขึ้น

### ข้อสังเกต
- โค้ดนี้แสดงให้เห็นการสร้าง inner page table แบบ on-demand เฉพาะเมื่อมีการเข้าถึง outer page นั้น ๆ
- การจัดการ frame เพราะ `frame_allocated` ทำให้ไม่เลือกรูปเดิมซ้ำกัน
- ในตอนท้ายโค้ดพยายาม free หน่วยความจำ แต่มี bug เล็กน้อยใน `free(outer_page_number[i]);` ซึ่งควรเป็น `free(outer_page_table[i]);`

## ความแตกต่างระหว่าง `paging_1level.c` และ `paging_2level.c`

- `paging_1level.c`
  - ใช้ตารางหน้าเดียว
  - page table ขนาดคงที่สำหรับทุก page
  - เหมาะกับระบบที่จำนวน pages ไม่มาก

- `paging_2level.c`
  - ใช้ตารางหน้าสองระดับ
  - สร้าง inner page table เฉพาะเมื่อจำเป็น
  - ลดการใช้หน่วยความจำสำหรับ page table เมื่อ address space ใหญ่

## สรุป
Activity 8 ทำให้เห็นหลักการพื้นฐานของ paging:
- การแยกที่อยู่เสมือนเป็น page + offset
- การใช้ page table เพื่อแปลงเป็นที่อยู่กายภาพ
- การเพิ่มความซับซ้อนของโครงสร้างข้อมูลจากหนึ่งระดับเป็นสองระดับ เพื่อประหยัดหน่วยความจำสำหรับตารางหน้า

ไฟล์ `act8.md` นี้สรุปทั้งหลักการและการทำงานของโค้ดทั้งสองไฟล์ให้ชัดเจนในเชิง process และ data structure

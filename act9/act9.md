# Activity 9: Virtual Memory and Page Faults

## Summary of PDF: activity 9 virtual memory.pdf

เอกสารนี้ครอบคลุมแนวคิดพื้นฐานของ Virtual Memory รวมถึง:
- การทำงานของ Page Faults และการจัดการหน่วยความจำเสมือน
- อัลกอริทึมการแทนที่หน้า (Page Replacement Algorithms) เช่น FIFO และ LRU
- การคำนวณ Page Fault Rate และผลกระทบต่อประสิทธิภาพระบบ

## Summary of Code: pagefault_assignment.c

โปรแกรมนี้เป็นการจำลองระบบ Paging เพื่อคำนวณ Page Fault Rate จากชุดการอ้างอิงหน้า (Page Reference String) ที่ผู้ใช้ป้อนเข้ามา

### โครงสร้างข้อมูลหลัก
- **Page Table**: อาร์เรย์ขนาด 128 ของ `PageTableEntry` ซึ่งประกอบด้วย:
  - `valid`: บิตที่บอกว่าหน้านี้อยู่ในหน่วยความจำจริงหรือไม่
  - `frame`: หมายเลขเฟรมที่หน้าเก็บอยู่
- **Frames**: อาร์เรย์ขนาด 128 ของ `FrameEntry` ซึ่งประกอบด้วย:
  - `page_number`: หมายเลขหน้าที่เก็บในเฟรมนี้ (-1 ถ้าเฟรมว่าง)
  - `timestamp`: เวลาที่หน้าถูกอ้างอิงล่าสุด (ใช้สำหรับ LRU)

### ฟังก์ชันสำคัญ
- `get_free_frame(int page_number, int timestamp)`: จัดการการจัดสรรเฟรมใหม่
  - ถ้ามีเฟรมว่าง: จัดสรรเฟรมว่างให้
  - ถ้าไม่มี: เลือกเฟรมที่จะแทนที่ตามอัลกอริทึม (FIFO หรือ LRU)

### ส่วนที่เน้น: Comments // Assignment
1. **Assignment 1.1**: เมื่อพบเฟรมว่าง ให้อัปเดต `frames[i].page_number = page_number`, `frames[i].timestamp = timestamp`, และลด `num_free_frames` ลง 1

2. **Assignment 1.2**: ค้นหาเฟรมที่เก่าที่สุด (timestamp น้อยที่สุด) เพื่อแทนที่

3. **Assignment 1.3**: ทำให้หน้าเดิมที่ถูกแทนที่ใน Page Table เป็น invalid (`page_table[replaced_page].valid = 0`)

4. **Assignment 1.4**: กำหนดหมายเลขหน้าและ timestamp ให้กับเฟรมที่เลือก (`frames[oldest_frame].page_number = page_number`, `frames[oldest_frame].timestamp = timestamp`)

5. **Assignment 2**: ในโหมด LRU เมื่อเกิด Page Hit ให้อัปเดต timestamp ของเฟรมที่เกี่ยวข้องเป็นค่า `page_references` ปัจจุบัน

### การทำงานโดยรวม
1. อ่านจำนวนเฟรมและ Page Reference String จากผู้ใช้
2. เริ่มต้น Page Table และ Frames
3. สำหรับแต่ละหมายเลขหน้าใน Reference String:
   - เพิ่มจำนวนการอ้างอิง (`page_references++`)
   - ตรวจสอบใน Page Table ถ้าไม่ valid (Page Fault):
     - เพิ่มจำนวน Page Faults
     - เรียก `get_free_frame` เพื่อจัดสรรเฟรม
     - อัปเดต Page Table ให้ valid และชี้ไปที่เฟรมนั้น
   - ถ้า valid (Page Hit):
     - ในโหมด LRU: อัปเดต timestamp ของเฟรม
4. คำนวณและแสดง Page Fault Rate: `(page_faults / page_references) * 100%`

โปรแกรมรองรับโหมด Verbose (-v) เพื่อแสดงผลการอ้างอิงแต่ละครั้ง และเลือกอัลกอริทึม (-a fifo หรือ lru)
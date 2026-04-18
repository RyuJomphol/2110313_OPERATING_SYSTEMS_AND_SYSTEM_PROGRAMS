# สรุป Activity 10: ระบบไฟล์ (Activity 10: Filesystem)

## วัตถุประสงค์
Activity 10 เป็นกิจกรรมเกี่ยวกับการศึกษาระบบไฟล์ (Filesystem) โดยเฉพาะการทำความเข้าใจเกี่ยวกับ:
- โครงสร้างและการจัดการไฟล์ในระบบ
- Inode (Index Node) และการทำงานของระบบไฟล์
- Hard link และ Symbolic link
- Block size และการจัดเก็บข้อมูล

## ขั้นตอนการทดลอง (File Writing Process - image1.png to image4.png)

### 1. ขั้นตอนแรก (image1.png):
- สร้างไดเรกทอรี tmp โดยใช้คำสั่ง `mkdir tmp`
- สร้างไดเรกทอรี mydir ใน tmp
- สร้างไฟล์ test files:
  - `test.txt` - ไฟล์ข้อมูล
  - `test.h` และ `test.s` - ไฟล์ลิงก์
- ใช้คำสั่ง `ln -s` สร้าง symbolic link
- ดูข้อมูลไฟล์โดยใช้คำสั่ง `stat`

### 2. ขั้นตอนที่สอง (image2.png):
- ใช้คำสั่ง `stat test.txt` เพื่อดูข้อมูลรายละเอียดของไฟล์:
  - File: test.txt
  - Size: 6150 bytes
  - Blocks: 16 (แต่ละ block 4096 bytes)
  - IO Block: 4096 (regular file)
  - Inode: 12466 และ 21608 (สำหรับไฟล์ต่างๆ)
  - Permissions: (0644 rw-r--r--)
  - Links: 2 (แสดงว่ามี hard link)

### 3. ขั้นตอนที่สาม (image3.png):
- ใช้คำสั่ง `ls -li` ในไดเรกทอรี /proc/311 เพื่อดูรายการไฟล์ที่มี inode numbers
- แสดงไฟล์ต่างๆ เช่น arch_status, attr, auxv, cgroup, clear_refs, cmdline, comm, coredump_filter, cpuset ฯลฯ
- แสดง inode numbers ตัวเลขทั้งหมดเพื่อให้เข้าใจโครงสร้าง

### 4. ขั้นตอนที่สี่ (image4.png):
- ใช้คำสั่ง `stat -f` เพื่อดูข้อมูล filesystem
- ดูข้อมูล /proc/311 และ fd (file descriptor)
- แสดงการสร้าง symbolic link ไปยัง /dev/pts/0

## คำตอบของคำถาม (imageMCV.png)

1. **ข้อมูลไฟล์ (File Information):**
   - File ID: 69bfda26fe791732
   - NameLen: 255 (ความยาวชื่อไฟล์สูงสุด)
   - Type: ext2/ext3 (ประเภทระบบไฟล์)
   - Block size: 4096 bytes (4 KB)
   - Total blocks: 263940717
   - Free blocks: 263337338
   - Available: 249911466
   - Total inodes: 67108864
   - Free inodes: 67052339

2. **Hard Link (ลิงก์แข็ง):**
   - Hard link ใช้เพื่อให้ไฟล์เดียวกันมีหลายชื่อ
   - มี inode เดียวกัน
   - ทั้งหมดชี้ไปที่เนื้อหาข้อมูลเดียวกัน
   - สามารถใช้ได้กับไฟล์เท่านั้น ไม่ใช้กับไดเรกทอรี

3. **Symbolic Link (ลิงก์สัญลักษณ์):**
   - Symbolic link เป็นชนิดของ reference ที่ชี้ไปยัง pathname ของไฟล์อื่น
   - มี inode ของตัวเอง
   - สามารถใช้ได้กับไฟล์และไดเรกทอรี
   - สามารถข้ามระบบไฟล์ได้

4. **Hard Link in Directory (ลิงก์แข็งในไดเรกทอรี):**
   - Hard link สำหรับไดเรกทอรี์ใช้เพื่อเป็น loop prevention mechanism
   - ใช้สำหรับการนำทางในโครงสร้างไดเรกทอรี

5. **Block Size (ขนาด Block):**
   - Block size = 4096 bytes (4 KB block)
   - นี่คือหน่วยพื้นฐานของการจัดเก็บข้อมูลในระบบไฟล์
   - แต่ละ block สามารถเก็บข้อมูลได้ 4096 bytes

## สรุปหลักของ Activity 10:

Activity 10 สำคัญต่อการเข้าใจ:
- ✅ Inode: โครงสร้างข้อมูลที่เก็บข้อมูลเมตาดาต้าของไฟล์
- ✅ Hard Link: การสร้างอีกชื่อหนึ่งสำหรับไฟล์เดียวกัน (inode เดียวกัน)
- ✅ Symbolic Link: การสร้าง reference ไปยังไฟล์อื่น (inode ต่างกัน)
- ✅ Block: หน่วยพื้นฐานของการจัดเก็บ (4096 bytes)
- ✅ Filesystem Structure: วิธีการจัดระเบียบข้อมูลในระบบ ext2/ext3

งานนี้สาธิตการใช้คำสั่ง Linux ต่างๆ เช่น `mkdir`, `ln`, `stat`, `ls -li` เพื่อศึกษาและวิเคราะห์โครงสร้างระบบไฟล์และ inode ของระบบปฏิบัติการ
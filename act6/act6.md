# Activity 6: Process Scheduling

## วัตถุประสงค์
กิจกรรมนี้มีวัตถุประสงค์เพื่อศึกษาการทำงานของ Process Scheduling โดยใช้ simulator ในการจำลอง algorithm ต่างๆ เช่น FCFS (First-Come-First-Served) และ SJF (Shortest-Job-First)

## ขั้นตอนการทำกิจกรรม
1. เข้าไปที่โฟลเดอร์ ps และรันคำสั่ง simulator
2. กำหนด parameter ในไฟล์ .exp และ .run
   - interarrival: ระยะห่างระหว่างเวลาที่โปรเซสเข้ามา (กำหนดเป็น probability)
   - duration: ระยะเวลาทั้งหมดที่โปรเซสใช้ (กำหนดเป็น probability)
   - cpuburst: ระยะเวลาการใช้ CPU ต่อครั้ง (CPU burst time)
   - ioburst: ระยะเวลาการใช้ I/O ต่อครั้ง (I/O burst time)
3. ตัวอย่างไฟล์ myexp.exp:
   - กำหนดการจำลอง 30 โปรเซส แบ่งเป็นสองกลุ่ม
   - กลุ่มแรก: duration 10-15 units, cpuburst 10 units, firstarrival 0, ioburst exponential
   - กลุ่มที่สอง: duration 50 units, cpuburst uniform, firstarrival 0, interarrival constant 0
4. ไฟล์ myrun.run: กำหนดจำนวนรันและ algorithm (FCFS และ SJF)
5. รัน simulator โดยกดปุ่ม "Run Experiment"
6. ดูผลลัพธ์:
   - กด "Show All Table Data" เพื่อดูสถิติต่างๆ
   - กด "Draw Gantt Chart" เพื่อดูกราฟแสดงสถานะของแต่ละโปรเซส

## ผลลัพธ์และการวิเคราะห์
- สำหรับ SJF: โปรเซสที่มี CPU burst เล็กจะทำงานเสร็จก่อน แม้จะมาทีหลัง
- เปรียบเทียบกับ FCFS: SJF ให้ waiting time และ turnaround time น้อยกว่า โดยเฉพาะสำหรับ CPU-bound processes
- มีสองประเภทโปรเซส: CPU-bound (ทำงาน CPU นาน) และ I/O-bound (ใช้ I/O บ่อย)
- จากตารางและ Gantt Chart: SJF มีประสิทธิภาพดีกว่า FCFS ในแง่ของเวลาเฉลี่ย

## สรุป
Activity นี้แสดงให้เห็นความแตกต่างระหว่าง scheduling algorithm ต่างๆ โดย SJF เหมาะสำหรับการลดเวลา waiting และ turnaround เมื่อเปรียบเทียบกับ FCFS
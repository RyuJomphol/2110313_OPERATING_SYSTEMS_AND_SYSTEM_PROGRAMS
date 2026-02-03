# Part 3

โค้ดนี้เป็นโปรแกรม C ที่แสดงการรวมกันระหว่างการใช้ process (ด้วย fork) และเธรด (ด้วย pthread) โดยทั้ง parent process และ child process จะสร้างเธรดของตัวเอง

## รายละเอียดโค้ด

- **Header files ที่ใช้:**
  - `<stdio.h>`: สำหรับฟังก์ชัน I/O เช่น printf
  - `<stdlib.h>`: สำหรับฟังก์ชันทั่วไป
  - `<pthread.h>`: สำหรับการจัดการเธรด
  - `<unistd.h>`: สำหรับฟังก์ชัน fork และอื่นๆ
  - `<sys/wait.h>`: สำหรับฟังก์ชัน wait

- **ฟังก์ชัน `thread_task`:**
  - รับพารามิเตอร์เป็น `void* message`
  - แปลงเป็น `char*` และพิมพ์ข้อความออกมา
  - จบเธรดด้วย `pthread_exit(NULL)`

- **ฟังก์ชัน `main`:**
  - ประกาศตัวแปร `pid_t pid`
  - เรียก `fork()` เพื่อสร้าง child process
  - **ใน child process (pid == 0):**
    - สร้างสองเธรด `t1` และ `t2` โดยเรียก `pthread_create` ส่งฟังก์ชัน `thread_task` และข้อความเกี่ยวกับ child process
    - ใช้ `pthread_join` เพื่อรอให้ทั้งสองเธรดเสร็จ
    - จบ child process ด้วย `exit(0)`
  - **ใน parent process (pid > 0):**
    - สร้างสองเธรด `t1` และ `t2` เช่นเดียวกับ child แต่ด้วยข้อความเกี่ยวกับ parent process
    - ใช้ `pthread_join` เพื่อรอให้ทั้งสองเธรดเสร็จ
    - เรียก `wait(NULL)` เพื่อรอให้ child process จบ
  - จบโปรแกรมด้วย `return 0`

## วัตถุประสงค์
โปรแกรมนี้แสดงให้เห็นการทำงานของ process และเธรดร่วมกัน โดย parent และ child process แต่ละตัวจะสร้างเธรดสองตัวที่พิมพ์ข้อความเพื่อแสดงความแตกต่างระหว่าง process ต่างๆ
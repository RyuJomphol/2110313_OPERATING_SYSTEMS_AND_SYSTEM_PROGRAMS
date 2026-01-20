# Activity 3: Process Creation with fork()

กิจกรรมนี้มีวัตถุประสงค์เพื่อให้น้อง ๆ เข้าใจการสร้างโปรเซสหลายระดับในระบบปฏิบัติการ Unix/Linux โดยใช้คำสั่ง `fork()` พร้อมทั้งเรียนรู้การตรวจสอบ **PID** และ **PPID** ของแต่ละโปรเซส

---

## 1. อธิบายคำสั่งที่ใช้

### 1.1 `fork()`

`fork()` เป็น system call ที่ใช้สำหรับสร้างโปรเซสใหม่ โดยโปรเซสใหม่ที่ถูกสร้างขึ้นจะเรียกว่า **child process** ซึ่งเป็นสำเนาของโปรเซสเดิม (**parent process**)

ค่าที่ `fork()` ส่งกลับมา:

* ค่าติดลบ (< 0) : การสร้างโปรเซสล้มเหลว
* ค่า 0 : โค้ดส่วนนี้กำลังทำงานอยู่ใน **child process**
* ค่ามากกว่า 0 : โค้ดส่วนนี้กำลังทำงานอยู่ใน **parent process** และค่าที่ได้คือ PID ของ child

---

### 1.2 `getpid()`

`getpid()` ใช้สำหรับดึงค่า **Process ID (PID)** ของโปรเซสที่กำลังทำงานอยู่ในขณะนั้น

ตัวอย่าง:

* ใช้แสดงว่าโปรเซสปัจจุบันคือใครในระบบ

---

### 1.3 `getppid()`

`getppid()` ใช้สำหรับดึงค่า **Parent Process ID (PPID)** ซึ่งคือ PID ของโปรเซสแม่ที่สร้างโปรเซสปัจจุบันขึ้นมา

ตัวอย่าง:

* ใช้ตรวจสอบความสัมพันธ์ระหว่าง parent และ child process

---

## 2. อธิบายการทำงานของโค้ด

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
```

ส่วนนี้เป็นการ include library ที่จำเป็นสำหรับการทำงานกับ process

* `unistd.h` : ใช้สำหรับ `fork()`, `getpid()`, `getppid()`
* `sys/wait.h` : ใช้สำหรับ `wait()`

---

```c
printf("I am the parent process. My PID is %d\n", getpid());
pid1 = fork();
```

* เริ่มต้นโปรแกรมจะมีเพียง **parent process** เท่านั้น
* แสดง PID ของ parent
* จากนั้นเรียก `fork()` เพื่อสร้าง **child process**

---

```c
if (pid1 < 0) {
    fprintf(stderr, "Fork Failed");
}
```

* ตรวจสอบกรณีที่ `fork()` ล้มเหลว

---

```c
else if (pid1 == 0) {
    printf("I am the child process. My PID is %d and my parent's PID is %d\n", getpid(), getppid());
    pid2 = fork();
}
```

* ส่วนนี้จะทำงานเฉพาะใน **child process**
* แสดง PID ของ child และ PPID (ซึ่งคือ parent)
* จากนั้น child จะเรียก `fork()` อีกครั้งเพื่อสร้าง **grandchild process**

---

```c
else if (pid2 == 0) {
    printf("I am the grandchild process. My PID is %d and my parent's PID is %d\n", getpid(), getppid());
}
```

* ส่วนนี้ทำงานใน **grandchild process**
* แสดง PID ของ grandchild และ PPID (ซึ่งคือ child)

---

```c
else {
    wait(NULL);
}
```

* ใช้ `wait()` เพื่อให้ parent หรือ child รอจนกว่าโปรเซสลูกจะทำงานเสร็จ
* ป้องกันไม่ให้เกิด zombie process

---

## 3. สรุปโครงสร้างโปรเซส

ลำดับการสร้างโปรเซส:

1. Parent Process
2. Child Process (สร้างโดย Parent)
3. Grandchild Process (สร้างโดย Child)

แต่ละโปรเซสจะแสดง:

* บทบาทของตนเอง (parent / child / grandchild)
* PID ของตัวเอง
* PPID ของโปรเซสแม่

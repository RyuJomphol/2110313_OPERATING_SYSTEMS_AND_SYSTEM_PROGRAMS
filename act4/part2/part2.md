# กิจกรรมที่ 2 Simple Shell (mysh)

## วัตถุประสงค์ของโปรแกรม

โปรแกรมนี้จำลองการทำงานของ shell อย่างง่าย โดยมีพฤติกรรมดังนี้

* แสดง prompt `mysh >`
* รับคำสั่งจากผู้ใช้ทางแป้นพิมพ์
* สร้าง process ลูก (child process) เพื่อรันคำสั่ง Linux
* process แม่ (parent process) รอให้ลูกทำงานเสร็จ
* ทำงานวนซ้ำไปเรื่อย ๆ
* หยุดการทำงานเมื่อผู้ใช้พิมพ์คำสั่ง `exit`

---

## 1. อธิบายฟังก์ชันที่ใช้ในโปรแกรม

### 1.1 `fgets()`

ใช้สำหรับรับข้อความจากผู้ใช้ทางแป้นพิมพ์

```c
fgets(buffer, 256, stdin);
```

* อ่านข้อมูลจาก `stdin`
* เก็บไว้ในตัวแปร `buffer`
* ปลอดภัยกว่า `gets()` เพราะกำหนดขนาดสูงสุดได้

---

### 1.2 `tokenize()`

ฟังก์ชันนี้ใช้สำหรับ **แยกคำสั่ง (command)** ที่ผู้ใช้พิมพ์เข้ามาออกเป็น token เช่น

```text
ls -l /home
```

จะถูกแยกเป็น

```text
args[0] = "ls"
args[1] = "-l"
args[2] = "/home"
args[3] = NULL
```

เหตุผลที่ต้องแยก token เพราะ `execvp()` ต้องการข้อมูลในรูปแบบ array ของ string ที่ลงท้ายด้วย `NULL`

---

### 1.3 `fork()`

```c
pid = fork();
```

* ใช้สร้าง process ลูก
* มีค่าที่เป็นไปได้ 3 แบบ

  * `pid < 0` → สร้าง process ไม่สำเร็จ
  * `pid == 0` → โค้ดส่วนนี้ทำงานใน **Child Process**
  * `pid > 0` → โค้ดส่วนนี้ทำงานใน **Parent Process**

---

### 1.4 `execvp()`

```c
execvp(args[0], args);
```

* ใช้รันคำสั่ง Linux เช่น `ls`, `pwd`, `date`
* แทนที่ process เดิมของ child ด้วยโปรแกรมใหม่
* ถ้าสำเร็จ จะ **ไม่กลับมาทำงานบรรทัดถัดไป**
* ถ้าล้มเหลว แสดงว่าคำสั่งไม่ถูกต้อง

---

### 1.5 `wait()`

```c
wait(NULL);
```

* ใช้ใน Parent Process
* รอให้ Child Process ทำงานเสร็จก่อน
* ป้องกันไม่ให้เกิด Zombie Process

---

### 1.6 `strcmp()`

```c
if (strcmp(args[0], "exit") == 0)
```

* ใช้เปรียบเทียบ string
* ตรวจสอบว่าผู้ใช้พิมพ์คำสั่ง `exit` หรือไม่

---

## 2. อธิบายจุดที่แก้และโครงสร้างการทำงานใน `main()`

### 2.1 ตัวแปรที่เพิ่มเข้ามา

```c
char buffer[256];
char delim[] = " \t\n";
char **args;
int numtokens;
pid_t pid;
```

* `buffer` : เก็บข้อความจากผู้ใช้
* `delim` : ตัวคั่นสำหรับแยกคำสั่ง
* `args` : array ของ token สำหรับส่งให้ `execvp()`
* `numtokens` : จำนวน token ที่แยกได้
* `pid` : เก็บ process ID จาก `fork()`

---

### 2.2 ลูปหลักของ Shell

```c
while(run) {
    printf("mysh >");
    fgets(buffer, 256, stdin);
    ...
}
```

* ทำให้ shell พร้อมรับคำสั่งตลอดเวลา
* จบลูปเมื่อผู้ใช้พิมพ์ `exit`

---

### 2.3 การตรวจสอบคำสั่งว่าง

```c
if (numtokens <= 0) {
    continue;
}
```

* กรณีผู้ใช้กด Enter เปล่า ๆ
* ไม่ต้อง fork process ใหม่

---

### 2.4 การตรวจสอบคำสั่ง `exit`

```c
if (strcmp(args[0], "exit") == 0) {
    run = 0;
}
```

* เปลี่ยนค่า `run` เป็น 0
* ทำให้ `while(run)` หยุดทำงาน

---

### 2.5 การทำงานของ Child Process

```c
if (pid == 0) {
    execvp(args[0], args);
    printf("Command not found: %s\n", args[0]);
    exit(1);
}
```

* Child รับหน้าที่รันคำสั่ง Linux
* ถ้าคำสั่งผิด จะพิมพ์ข้อความแจ้งเตือน

---

### 2.6 การทำงานของ Parent Process

```c
else {
    wait(NULL);
}
```

* Parent รอให้ Child ทำงานเสร็จ
* จากนั้นกลับไปรับคำสั่งใหม่

---
# x86script
meme assembler

```x86asm
%externdef user32:MessageBoxW

push ebp
mov ebp, esp

push 0
push msg1
push msg1
push 0
call MessageBoxW

push 0
push msg2
push msg2
push 0
call MessageBoxW

pop ebp
ret

msg1: dw "pog", 0
msg2: dw "pogs", 0
```

![s](https://media.discordapp.net/attachments/586656439129341963/784182964069466152/unknown.png)

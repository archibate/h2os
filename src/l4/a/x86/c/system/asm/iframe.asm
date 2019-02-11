bits 32
section .text

globl move_to_user
globl utcb_iframe_exiter
globl iframe_exiter
extrn hwintr

move_to_user:
	mov esp, [esp + 4]
	jmp iframe_exiter

SEFrameSize equ 4*4 ; N: sizeof(struct seframe): keep sync with l4/system/asm/seframe.h

introute:
	push ds
	push es
	push fs
	push gs
	pushad
	mov eax, ss
	mov ds, eax
	mov es, eax
	mov ecx, esp
	sub esp, SEFrameSize
	call hwintr
utcb_iframe_exiter:
	add esp, SEFrameSize
iframe_exiter:
	popad
	pop gs
	pop fs
	pop es
	pop ds
	add esp, 8 ; intr_num & error_code
	iretd

section .data
globl __intrents
__intrents:

%macro NC 1
section .data
  dd ent%1
section .text
ent%1:
  push 0
  push %1
  jmp introute
%endmacro

%macro EC 1
section .data
  dd ent%1
section .text
ent%1:
  push %1
  jmp introute
%endmacro

NC 0
NC 1
NC 2
NC 3
NC 4
NC 5
NC 6
NC 7
EC 8
NC 9
EC 10
EC 11
EC 12
EC 13
EC 14
NC 15
NC 16
NC 17
NC 18
NC 19
NC 20
NC 21
NC 22
NC 23
NC 24
NC 25
NC 26
NC 27
NC 28
NC 29
NC 30
NC 31
NC 32
NC 33
NC 34
NC 35
NC 36
NC 37
NC 38
NC 39
NC 40
NC 41
NC 42
NC 43
NC 44
NC 45
NC 46
NC 47
NC 48
NC 49
NC 50
NC 51
NC 52
NC 53
NC 54
NC 55
NC 56
NC 57
NC 58
NC 59
NC 60
NC 61
NC 62
NC 63
NC 64
NC 65
NC 66
NC 67
NC 68
NC 69
NC 70
NC 71
NC 72
NC 73
NC 74
NC 75
NC 76
NC 77
NC 78
NC 79
NC 80
NC 81
NC 82
NC 83
NC 84
NC 85
NC 86
NC 87
NC 88
NC 89
NC 90
NC 91
NC 92
NC 93
NC 94
NC 95
NC 96
NC 97
NC 98
NC 99
NC 100
NC 101
NC 102
NC 103
NC 104
NC 105
NC 106
NC 107
NC 108
NC 109
NC 110
NC 111
NC 112
NC 113
NC 114
NC 115
NC 116
NC 117
NC 118
NC 119
NC 120
NC 121
NC 122
NC 123
NC 124
NC 125
NC 126
NC 127
NC 128
NC 129
NC 130
NC 131
NC 132
NC 133
NC 134
NC 135
NC 136
NC 137
NC 138
NC 139
NC 140
NC 141
NC 142
NC 143
NC 144
NC 145
NC 146
NC 147
NC 148
NC 149
NC 150
NC 151
NC 152
NC 153
NC 154
NC 155
NC 156
NC 157
NC 158
NC 159
NC 160
NC 161
NC 162
NC 163
NC 164
NC 165
NC 166
NC 167
NC 168
NC 169
NC 170
NC 171
NC 172
NC 173
NC 174
NC 175
NC 176
NC 177
NC 178
NC 179
NC 180
NC 181
NC 182
NC 183
NC 184
NC 185
NC 186
NC 187
NC 188
NC 189
NC 190
NC 191
NC 192
NC 193
NC 194
NC 195
NC 196
NC 197
NC 198
NC 199
NC 200
NC 201
NC 202
NC 203
NC 204
NC 205
NC 206
NC 207
NC 208
NC 209
NC 210
NC 211
NC 212
NC 213
NC 214
NC 215
NC 216
NC 217
NC 218
NC 219
NC 220
NC 221
NC 222
NC 223
NC 224
NC 225
NC 226
NC 227
NC 228
NC 229
NC 230
NC 231
NC 232
NC 233
NC 234
NC 235
NC 236
NC 237
NC 238
NC 239
NC 240
NC 241
NC 242
NC 243
NC 244
NC 245
NC 246
NC 247
NC 248
NC 249
NC 250
NC 251
NC 252
NC 253
NC 254
NC 255
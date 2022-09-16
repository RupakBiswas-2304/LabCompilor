; bubble sort in C
; void bubble_sort(int* array, int n) {
;     int i, j, temp;
;     for (i = 0; i < n; i++) {
;         for (j = 0; j < n - 1; j++) {
;             if (array[j] > array[j + 1]) {
;                 temp = array[j];
;                 array[j] = array[j + 1];
;                 array[j + 1] = temp;
;             }
;         }
;     }
; }

bubbleSort:    
    LOOP:
        ldc total   ; A = &total
        ldnl 0      ; A = total
        ldc i       ; A = &j ,B = total
        ldnl 0
        sub         ; A = total - i
        ;ldc 1       ; A = 1 ,B = total - i
        ;sub         ; A = total - i - 1
        brz END     ; breaking loop if i == total
        LOOP2:
            ldc total               ;
            ldnl 0
            ldc j                   ; A = j, B = total
            ldnl 0
            sub                     ; A = total - j
            brz ResetJ              ; start LOOP again if j == total with i++
            ;-----------------------
            ;-----------------------   j2 = j + 1 (temporary storing j+1)
            ldc j                      ; A = &j
            ldnl 0
            ldc 1                      ; A = 1, B = j
            add                        ; A = j + 1
            ldc j2                     ; A = &j2, B = j + 1
            stnl 0                     ; j2 = j + 1
            ;-------------------------
            ;------------------------- if (array[j+1] -array[j] < 0)
            ldc array                   ; A = &array
            ldnl j                      ; A = array[j]
            ldc 0                       ; A = 0, B = array[j]
            ldc array                   ; A = array, B = array[i]
            ldnl j2                     ; A = array[j+1], B = array[i]
            sub                         ; A = array[j] - array[j+1]
            brlz PASS                   ; if A < 0, PASS else swap
            ;-------------------------- swap
            ldc array                   ; A = array
            ldnl j                      ; A = array[j]
            ldc temp                    ; A = temp, B = array[j]
            stnl 0                      ; temp = array[j]

            ldc array                   ;A = array
            ldnl j2                     ; A = array[j+1], B = array[j]
            ldc temp                   ; A = &temp, B = array[j+1]
            stnl j                      ; array[j] = array[j+1]

            ldnl 0                      ; A = temp, B = array[j+1]
            ldc array                   ; A = array, B = temp
            stnl j2                     ; array[j+1] = temp
            ;------------ (j++)
            PASS:
                ldc 1                   ; A = 1
                ldc j                   ; A = &j, B = 1
                ldnl 0
                add                     ; A = j + 1
                ldc j                   ; B = j + 1 , A = &j
                ldnl 0
                stnl 0                  ; j = j + 1
                br LOOP2

ResetJ:                     ; reset j to 0
    ldc 0                   ; A = 0
    ldc j                   ; A = j, B = 0
    ldnl 0
    stnl 0                  ; j = 0
    ldc 1                   ; A = 1
    ldc i                   ; A = i, B = 1
    ldnl 0
    add                     ; A = i + 1
    ldc i                   ; A = i , B = i + 1
    stnl 0                  ; i = i + 1
    br LOOP

END: HALT


;------------------------ DATA --------------------
i: data 0
j: data 0
j2: data 0
temp: data 0
total: data 6
StackStart: data 80
offset: data 0
array: data 5
data 14
data 10
data 7
data 8
data 5


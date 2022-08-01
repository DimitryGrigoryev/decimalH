#include "s21_decimal.h"

#include "limits.h"

void Ya_print_decimal(s21_decimal dec) {
  char sign;
  if (get_sign(dec))
    sign = '-';
  else
    sign = '+';
  printf("\n");
  for (int i = 0; i < 4; i++) {
    unsigned int fbits = *((unsigned int *)&dec.bits[i]);
    printf("\t");
    for (unsigned int mask = 0x80000000; mask; mask >>= 1) {
      if (i == 3 && (mask == 0x40000000 || mask == 0x800000 || mask == 0x8000))
        printf(" ");
      printf("%d", !!(fbits & mask));
    }
    printf("\t|\t");
    if (i == 3) {
      printf("Sign: '%c' / Scale: %d\n", sign, getDecimalScale(dec));
    } else {
      printf("%u", dec.bits[i]);
      printf("\n");
    }
  }
}

long double yaPow(long double num, int degr ) {
  long double temp = 0.0;
  temp = num;
  for (int i = 1; i < degr; i++) {
    temp *= num;
  }
  return temp;
}

void Ya_print_decimal_all(s21_decimal dec) {
    long double tmpNum = 0.0;
    long double numDec = 0.0;
    int tmpScale = getDecimalScale(dec);
    for (int i = 95; i >= 0; i--) {
        if (get_bit(i, dec) == 1) {
            if (i == 0 ) {
                tmpNum = 1;
            } else {
                tmpNum = yaPow(2.0, i);
            }
            numDec += tmpNum;
        }
    }
    if (tmpScale <= 28 && tmpScale > 0)  {
        for (int i = 1; i <= tmpScale; i++) {
            numDec /= 1e+1;
        }
    } else if (tmpScale > 28) {
        int err = 1;
    }
    printf("num: %.28Lf \n", numDec );
}

int main() {
  // s21_decimal src1, src2, dst = {4294967295,4294967295,4294967295,0};
  // s21_decimal src1, src2;
  int num = 0;
  s21_decimal src;
    src.bits[0] = 0b00000000000000000000000000110010;
    src.bits[1] = 0b00000000000000000000000000000000;
    src.bits[2] = 0b00000000000000000000000000000000;
    src.bits[3] = 0b00000000000000010000000000000000;

  printf("exit code: %d\n", s21_from_decimal_to_int(src, &num));
  // s21_decimal src1 = {{5000, 0, 0, 0}};
  // src1.bits[3] = 0b10000000000000000000000000000000;
  // s21_decimal src2 = {{100, 0, 0, 0}};
  // Ya_print_decimal(src1);
  // Ya_print_decimal(src2);
  // printf("exit code: %d\n",s21_add(src1, src2, &dst));
  Ya_print_decimal(src);
  printf("%d\n", num);
  // Ya_print_decimal_all(src1);
  // Ya_print_decimal_all(src2);
  Ya_print_decimal_all(src);
  // printf("decimal_compare: %d\n",decimal_compare(src1, src2));
  // printf("exit code: %d",s21_is_less_or_equal(&src1, &src2));
  // setEqualScale(&src1, &src2);
  // print_decimal(&src1);
  // printf("\n");
  // print_decimal(&src2);
  // printf("result decimal: \n");
  // print_decimal(&dst);
  // big_decimal  src1, src2, dst, ost;
  // big_decimal_init(&src1);
  // big_decimal_init(&src2);
  // big_decimal_init(&dst);
  // src1.bits[0] = 0b01111111111111111111111111111111;
  // src2.bits[0] = 0b10000000000000000000000000000000;
  // ost = simple_div_big(src1, src2, &dst);
  // printf("\n");
  // print_big_decimal(&src1);
  // printf("\n");
  // print_big_decimal(&src2);
  // printf("result decimal: \n");
  // print_big_decimal(&dst);
  // printf("ostatok decimal: \n");
  // print_big_decimal(&ost);
}

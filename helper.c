//
// Created by hod on 30/03/18.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "main.h"



struct bignum* convertTObignum(long array[],long size){
    bool isNegative=array[0]==-1;
    struct bignum* num=calloc(1,sizeof(*num));
    if (num==NULL)
        exit(-1);
    num->digit = calloc((size_t)size*9+1,sizeof(char));
    if(num->digit==NULL){
        free(num);
        exit(-1);
    }
    num->capacity = size*9+1;
    char* temp= num->digit;
    num->digit[size*9]='\0';
    for (long i=0;i<size-1;i++,temp+=9) {
        sprintf(temp, "%09ld", array[size-1-i]);
    }
    long i=0;
    for(;i<size*9&&num->digit[i]=='0';i++){}// number of '0' in the beggining of the string
    long k=0;
    for(;(k<size*9)&&(num->digit[k]>=48&&num->digit[k]<=57);k++){}// length of string
    num->numberOfDigits= k-i;
    if(num->numberOfDigits==0)
        i--;
    char*newdigit =calloc((size_t)num->numberOfDigits+2,sizeof(char));
    if(newdigit==NULL){
        exit(-1);
    }
    for(long j = 0;num->digit[i+j]!='\0';j++){
        newdigit[j]=num->digit[i+j];
        newdigit[j+1]='\0';
    }
    free(num->digit);
    num->digit=newdigit;
    num->sign=1;
    if (isNegative){
        num->sign=-1;
    }
    free(array);

    return num;

}
struct bignum* convertTObignumWithoutFree(long array[],long size){
    bool isNegative=array[0]==-1;
    struct bignum* num=calloc(1,sizeof(*num));
    if (num==NULL)
        exit(-1);
    num->digit = calloc((size_t)size*9+1,sizeof(char));
    if(num->digit==NULL){
        free(num);
        exit(-1);
    }
    num->capacity = size*9+1;
    char* temp= num->digit;
    num->digit[size*9]='\0';
    for (long i=0;i<size-1;i++,temp+=9) {
        sprintf(temp, "%09ld", array[size-1-i]);
    }
    long i=0;
    for(;i<size*9&&num->digit[i]=='0';i++){}
    long k=0;
    for(;(k<size*9)&&(num->digit[k]>=48&&num->digit[k]<=57);k++){}// length of string
    num->numberOfDigits= k-i;
    if(num->numberOfDigits==0)
        i--;
    char*newdigit =calloc((size_t)num->numberOfDigits+1,sizeof(char));
    if(newdigit==NULL){
        exit(-1);
    }
    for(long j = 0;num->digit[i+j]!='\0';j++){
        newdigit[j]=num->digit[i+j];
    }
    free(num->digit);
    num->digit=newdigit;
    num->sign=1;
    if (isNegative){
        num->sign=-1;
    }
    //free(array);

    return num;

}


long getLongValue(const char* digits, long end, long begin){
    long ans=0;
    for(;begin<=end;begin++){
        ans = ans*10 + (digits[begin]-'0');
    }
    return ans;
}
long *convertToArray(struct bignum* number){
    bool isNegative = number->sign==-1;
    long size = number->numberOfDigits/9+1;
    long* answer=calloc((size_t)size+2,sizeof(long));
    if(answer==NULL)
        exit(-1);
    long beginningOfLong=number->numberOfDigits-1;
    long endOfLong=beginningOfLong-8;
    if(isNegative)
        answer[0]=-1;
    else
        answer[0]=1;
    for(long j=1;j<size+1;j++,beginningOfLong-=9,endOfLong-=9){
        if(endOfLong<0)
            answer[j]=getLongValue(number->digit, beginningOfLong,0);
        else
            answer[j]=getLongValue(number->digit, beginningOfLong,endOfLong);
    }
    return answer;

}

void addDigit(char c, struct bignum* number) {
    number->digit[number->numberOfDigits]= c;
    number->numberOfDigits++;
    if(number->numberOfDigits==number->capacity-1){
        number->capacity= number->capacity+MAX_SIZE;
        char* newNumber;
        newNumber=(char*) calloc((size_t)number->capacity,sizeof(char));
        if(newNumber==NULL)
            exit(-1);
        char*j= newNumber;
        for(char* i=number->digit; i < number->digit + number->numberOfDigits; i++,j++){
            *j=*i;
        }
        free(number->digit);
        number->digit=newNumber;
    }
    if(c=='\0')
        number->numberOfDigits--;
}


struct bignum* calcSumWithoutFree(struct bignum* first,struct bignum* second) {
    long firstNewSize = first->numberOfDigits/9+1;
    long secondNewSize = second->numberOfDigits/9+1;;
    long max = secondNewSize;
    long min = firstNewSize;
    long* smaller = convertToArray(first);
    long* bigger = convertToArray(second);
    bool error = ((first)->sign==-2 || (second)->sign==-2);
    bool firstGreaterOrEqualtoSecond =compare(first,second)>0;
    if(firstGreaterOrEqualtoSecond){//if first number size is smaller then the second
        max=firstNewSize;
        min = secondNewSize;
        long * tmp = smaller;
        smaller = bigger;
        bigger = tmp;
    }
    long *result= calloc((size_t)max+1, sizeof(long));
    if(result==NULL)
        exit(-1);
    if(bigger[0]==smaller[0]) {
        addingTwoArrays(bigger+1, smaller+1, max, min,result);
        result[0]=bigger[0];
    }
    else{
        subTwoArrays(bigger+1,smaller+1,max,min,result);
        if(bigger[0]==-1){//if the bigger is negative
            result[0]=-1;
        }
        else{
            result[0]=1;
        }
    }
    free(bigger);
    free(smaller);
    struct bignum* answer = convertTObignum(result,max+1);
    if (error){
        answer->sign = -2;
    }
    return answer;
}
struct bignum* calcSubWithoutFree(struct bignum* first,struct bignum* second) {
    negateNumber(first);
    return calcSumWithoutFree(first,second);
}
int isEqualZeroOrSign(struct bignum** multiplier,struct bignum** multiplied, int multOrDiv ) {
    long multiplierNewSize = (*multiplier)->numberOfDigits / 9 + 1;
    long multipliedNewSize = (*multiplied)->numberOfDigits / 9 + 1;
    long *multiplierArr = convertToArray(*multiplier);
    long *multipliedArr = convertToArray(*multiplied);
    int con = 0;
    int con2 = 0;

    for (long i = 1; con != -1 && i <= multiplierNewSize; i++) {
        if (multiplierArr[i] != 0)
            con = -1;
    }

    if (con != 0) {
        con = 0;
        con2 = 0;
        for (long i = 1; con != -1 && i <= multipliedNewSize; i++) {
            if (multipliedArr[i] != 0) {
                con = -1;
                con2 = -1;
            }
        }

    if (multOrDiv == 1 && con2 == 0 && con==0) {
        con = -2;
        }
    }
    if (((*multiplier)->sign == -2) ||((*multiplied)->sign == -2)){
        con = -2;
    }

    if (con ==-1){
        con = ((*multiplied)->sign) * ((*multiplier)->sign);
            multipliedArr[0]=1;
            *multiplied = convertTObignum(multipliedArr,multipliedNewSize+1);
            (*multiplied)->sign = 1;
            multiplierArr[0]=1;
            *multiplier = convertTObignum(multiplierArr,multiplierNewSize+1);
            (*multiplier)->sign = 1;

    }
    //free(multiplierArr);
    //free(multipliedArr);
    return con;
}
struct bignum* returnZeroArray(){
    long *resultArr =  calloc(2, sizeof(long));
   // if(resultArr==NULL)
   //     exit(-1);
    resultArr[0] = 1;
    resultArr[1] = 0;
    return convertTObignum(resultArr,2);
}


void negateNumber(struct bignum *number) {
    number->sign=number->sign*(-1);
}
void printNumber(struct bignum *number) {
    printf("{\n\tsign: %d\n\tcapacity: %ld\n\tnumberOfDigits: %ld\n\tdigits: %s\n\t}\n",number->sign
            ,number->capacity,number->numberOfDigits, number->digit);
}
void freeBignum(struct bignum *number) {
    free(number->digit);
    free(number);
}
void freeStack(struct stack *s) {
    for (struct bignum** number=s->firstBignum;number<s->firstBignum+s->size;number++){
        freeBignum(*number);
    }
    s->size=0;
}

void minimizeBignumDigits(struct bignum *number) {
    long i=0;
    for(;i<number->numberOfDigits&&number->digit[i]=='0';i++){}
    if (i==number->numberOfDigits)
        i--;
    long k=0;
    for(;(k<number->numberOfDigits)&&(number->digit[k]>=48&&number->digit[k]<=57);k++){}// length of string
    number->numberOfDigits= k-i;

    char*newdigit =calloc((size_t)number->numberOfDigits+1,sizeof(char));
    if(newdigit==NULL)
        exit(-1);
    for(long j = 0;j<number->numberOfDigits;j++){
        newdigit[j]=number->digit[i+j];
        newdigit[j+1]='\0';
    }
    free(number->digit);
    number->digit=newdigit;
}


3
4
5
6
7
8
8
8
4
6
7
3
12
18
20
101
1167
0
3
3
4
8
-2
5
99
967
-10
1
1
-1
-1
8
8
-8
-8
-5
-5
-16
-13
98
-98
-102
-102
400000000000000000000000000000000000022
-400000000000000000000000000000000000022
400000000000000000000000000000000000022
-400000000000000000000000000000000000022
-3999999999999999999999999999999999999999999999990
3999999999999999999999999999999999999999999999990
5
-5
-5
5
22
-22
-22
22
-1
1
-14
-8
5
103
103
-97
97
3999999999999999999999999999999999999999999999990
-3999999999999999999999999999999999999999999999990
400000000000000000000000000000000000022
-400000000000000000000000000000000000022
-3999999999999999999999999999999999999999999999990
3999999999999999999999999999999999999999999999990
5
-5
-5
8
-8
-8
8
8
-8
0
0
14
-14
-14
14
48
-48
-48
48
48
-48
-48
48
783
-783
-783
783
783
-783
-783
783
510
-510
-510
510
510
-510
-510
510
88888888888888888888888888888888888888888888888888888888
-88888888888888888888888888888888888888888888888888888888
-88888888888888888888888888888888888888888888888888888888
88888888888888888888888888888888888888888888888888888888
88888888888888888888888888888888888888888888888888888888
-88888888888888888888888888888888888888888888888888888888
-88888888888888888888888888888888888888888888888888888888
88888888888888888888888888888888888888888888888888888888
8641975238641975238641975238641975238641975238641975230864197523
-8641975238641975238641975238641975238641975238641975230864197523
-8641975238641975238641975238641975238641975238641975230864197523
8641975238641975238641975238641975238641975238641975230864197523
8641975238641975238641975238641975238641975238641975230864197523
-8641975238641975238641975238641975238641975238641975230864197523
-8641975238641975238641975238641975238641975238641975230864197523
8641975238641975238641975238641975238641975238641975230864197523
12345678912344567890
123456789
5
0
3
2
0
0
-5
0
-3
-2
0
0
-5
0
-3
-2
0
0
5
0
3
2
0
0
3
-3
-3
3
6
4
3
2
-2
-2
2
0
-6
-4
-3
0
-6
-4
-3
0
6
4
3
0
333
-333
-333
333
249
-249
-249
249
0
0
0
0
2222222222222
-333333333333333333333333333333333333333333333333333333333333333
-333333333333333333333333333333333333333333333333333333333333333
-333333333333333333333333333333333333333333333333333333333333333
333333333333333333333333333333333333333333333333333333333333333
61728394561728394561728394561728394561728394561728394561728394
-61728394561728394561728394561728394561728394561728394561728394
-61728394561728394561728394561728394561728394561728394561728394
61728394561728394561728394561728394561728394561728394561728394
3
4
8
5
-1
38
1219326311126352690
40642538607
5
122
5
-12
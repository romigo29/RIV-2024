byte function getAnd(byte a,byte b){
return a&b;
}

int function getMul(int a,int b){
return a*b;
}

main{

let byte yesor=3|5;
let byte inversion=~4;
let byte yesand=getAnd(4,5)+getAnd(4,5);

print(yesor);
print(inversion);
print(yesand);

let byte rv=rest(yesor,yesand);
let byte mv=module(inversion);

print(rv);
print(mv);

let int iv=getMul(155,198);
print(iv);
let ch symbol='I';
let str stroka="dont mind";
print(symbol);
print("����� ������");

let bool flag=0;

if(inversion>2){
flag=1;
print(flag);
}
else{
print(flag);
}

print(yesor);

}
void pompa(){
  if (ta >= full){                                  //masuk
    digitalWrite(RLY2, HIGH);                   
  }

  if (ta <= full - (full * cond / 100)){          //keluar
    digitalWrite(RLY1, HIGH);                   
  }
}

BLYNK_WRITE(V4){
  int led1 = param.asInt();

  if (led1 == 0){
    btn1();
  }
}

BLYNK_WRITE(V5){
  int led2 = param.asInt();

  if (led2 == 0){
    btn2();
  }
}

void btn1() {                       //keluar
  if (cond == 30 || cond == 50){     
    if (ta >= full/2){
      digitalWrite(RLY1, LOW);   
    }             
  }
}

void btn2() {                        //masuk
  if (ta < full){
    digitalWrite(RLY2, LOW);                
  }
}

/* Протокол Нидхама-Шредера */
/* Три участника: Alice, Bob, и Intruder, которые обмениваются сообщениями друг
 * с другом по сети
 */
mtype = { msg, alice, bob, intruder, pkA, pkB, pkI,
          nonceA, nonceB, nonceI, ok, err };

/* По сети передается сообщение
 * (отправитель, получатель, №, зашифрованные данные (случайные числа или ID))
 */
typedef mesCrypt {
  mtype s, r, nummsg; /* открытая часть */
  mtype key, d1, d2;  /* зашифрованная часть */
};

/* Читающий из faked канала не подозревает, что сообщение было испорчено */
chan fakedA = [0] of { mtype, mesCrypt };
chan fakedB = [0] of { mtype, mesCrypt };

/* Пишущий в intercepted канал не знает, что сообщение будет прочитано */
chan intercepted = [0] of { mtype, mesCrypt };

/* Переменные, используемые в LTL формулах */
mtype partnerA, partnerB;
mtype statusA, statusB;

/* Переменные отражают знание посторонним секретных частей общего ключа */
bool knowNA, knowNB;

/* Честный инициатор процесса обмена */
active proctype Alice() {
  mtype pkey, pnonce;
  mesCrypt data;

  statusA = err;

  if
  :: partnerA = bob;      pkey = pkB;
  :: partnerA = intruder; pkey = pkI;
  fi;

  /* конструируется сообщение № 1 и отправляется */
  d_step {
    data.s = alice;
    data.r = partnerA;
    data.nummsg = 1;
    data.key = pkey;
    data.d1 = alice;
    data.d2 = nonceA;
  }
  intercepted ! msg, data;

  /* ожидает сообщения № 2 и расшифровывает его */
  fakedA ? msg, data;

end_errA:
  /* проверяет, что сообщение предназначалось именно ему и
   * что отправитель его партнер,
   * иначе останавливает процедуру обмена.
   */
  if
  :: (data.key == pkA) && (data.d1 == nonceA) &&
     (data.s == partnerA) && (data.r == alice) &&
     (data.nummsg == 2) ->
       pnonce = data.d2;
  :: else -> goto stopA;
  fi;

  /* отвечает сообщением №3 и успешно завершается */
  d_step {
    data.s = alice;
    data.r = partnerA;
    data.nummsg = 3;
    data.key = pkey;
    data.d1 = pnonce;
    data.d2 = 0;
  }
  intercepted ! msg, data;

  statusA = ok;

stopA:
  printf("MSC: Process A finished \n");
} /* proctype Alice() */

/* Честный партнер по обмену */
active proctype Bob() {
  mtype pkey, pnonce, receiver, partner;
  mesCrypt data;

  statusB = err;

  /* ожидает сообщения msg1, идентифицирует партнера */
  fakedB ? msg, data;

  /* проверяет сообщение на правильность построения,
   * если что-то не соответствует ожиданию, останавливается
   */
end_errB1:
  if
  :: (data.r == bob) && (data.key == pkB) && (data.s == data.d1) &&
     (data.nummsg == 1) ->
       partnerB = data.s;
       pnonce = data.d2;
  :: else -> goto stopB;
  fi;

  /* устанавливает публичный ключ партнера */
  if
  :: (partnerB == alice)    -> pkey = pkA;
  :: (partnerB == intruder) -> pkey = pkI;
  :: else -> goto stopB;
  fi;
/* отвечает сообщением № 2 */
  d_step {
    data.s = bob;
    data.r = partnerB;
    data.nummsg = 2;
    data.key = pkey;
    data.d1 = pnonce;
    data.d2 = nonceB;
  }
  intercepted ! msg, data;

  /* ожидает № 3, проверяет сообщение на корректность,
   * и в случае успеха завершается в состоянии ok
   */
  fakedB ? msg, data;

end_errB2:
  if
  :: (data.r == bob) && (data.s == partnerB) && (data.key == pkB) &&
     (data.d1 == nonceB) && (data.nummsg == 3) ->
       statusB = ok;
  :: else -> goto stopB;
  fi;

stopB:
  printf("MSC: Process B finished \n");
}

/* Злоумышленник не следует детерминированному алгоритму */
active proctype Intruder() {
  mesCrypt data, fake;

  knowNA = false;
  knowNB = false;

end:
  do
  :: intercepted ? msg, data ->
       if
       :: (data.key == pkI) ->
            if
            :: (data.d1 == nonceA || data.d2 == nonceA) -> knowNA = true;
            :: (data.d1 == nonceB || data.d2 == nonceB) -> knowNB = true;
            fi;
       :: else -> skip;
       fi;

       /* построение испорченного сообщения */
       if
       :: fake.s = alice ->
            fake.r = bob;
            fake.key = pkB;
       :: fake.s = bob ->
            fake.r = alice;
            fake.key = pkA;
       :: fake.s = intruder ->
            if
            :: fake.r = bob  -> fake.key = pkB;
            :: fake.r = alice -> fake.key = pkA;
            fi;
       fi;

       /* злоумышленник не знает правила построения сообщения */
       if
       :: fake.d1 = alice;
       :: fake.d1 = bob;
       :: fake.d1 = intruder;
       :: fake.d1 = nonceI;
       :: (knowNA) -> fake.d1 = nonceA;
       :: (knowNB) -> fake.d1 = nonceB;
       fi;

       if
       :: fake.d2 = alice;
       :: fake.d2 = bob;
       :: fake.d2 = intruder;
       :: fake.d2 = nonceI;
       :: (knowNA) -> fake.d2 = nonceA;
       :: (knowNB) -> fake.d2 = nonceB;
       fi;

       /* вставляет в сообщение неизвестную зашифрованную часть
        * из полученного сообщения
        */
       if
       :: (data.key != pkI) ->
            fake.key = data.key;
            fake.d1 = data.d1;
            fake.d2 = data.d2;
       :: else -> skip;
       fi;

       /* выбираем номер сообщения */
       if
       :: fake.nummsg = 1;
       :: fake.nummsg = 2;
       :: fake.nummsg = 3;
       fi;

       if
       :: (data.r == alice) ->
            fakedA ! msg, data; /* повторяем полученное сообщение */
       :: (data.r == bob) ->
            fakedB ! msg, data; /* повторяем полученное сообщение */
       :: (fake.r == alice) ->
            fakedA ! msg, fake; /* отправляем испорченное сообщение */
       :: (fake.r == bob) ->
            fakedB ! msg, fake; /* отправляем испорченное сообщение */
       fi;
  od;
}

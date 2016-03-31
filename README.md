# pmpaymasterua
Модуль интеграции Paymaster.ua и ISPsystem BILLmanager 5.

Для установки модуля на сервер нужно выполнить под пользователем root команду
```
wget https://raw.githubusercontent.com/bdolgov/pmpaymasterua/master/installer.sh -O - | bash -
```

После этого в BILLmanager в разделе "Методы оплаты" при создании появится новый метод PayMaster.UA.

На стороне PayMaster.Ua нужно указать следующие настройки мерчанта:
* Метод вызова URL страницы успешной оплаты: post
* Метод вызова URL страницы неуспешной оплаты: post
* URL страницы результата оплаты: https://*АдресВашегоБиллинга*/mancgi/paymasteruaresult
* Метод вызова URL страницы результата оплаты: post
* Метод формирования контрольной подписи: sha256
* Отправлять предзапрос (рекомендовано): да
* Проверять уникальность номера счета: нет
* Повторно отправлять Payment Notification при сбоях : да
* Включить проверку подписи в форме запроса платежа: да
* Протокол совместимости: paymaster

На стороне BILLmanager нужно указать следующие настройки:
* ID магазина (MERCHANT_ID): берётся из кабинета Paymaster
* Секретный ключ: берётся из кабинета Paymaster
* Метод оплаты: в случае выбора только одного метода оплаты, клиент будет автоматически переадресовываться на сайт этой платёжной системы. Если выбрано несколько методов оплаты или пункт "Все", клиент будет переадресовываться на сайт Paymaster для выбора необходимого метода оплаты. Вы можете создать различные методы оплаты в BILLmanager, отвечающие различным методам оплаты в Paymaster.
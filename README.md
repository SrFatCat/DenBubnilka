# Бубнилка в берлогу или WEB-радио с теплыми ламповыми часами.
Вот есть у меня берлога. И все в ней хорошо (лучше всего, что она есть), но бывает иногда одиноко там или просто хочется чтобы душа равернулась. Короче радио нужно. И понятно, есть у меня там старенький бук и в кармане всегда, телефон и можно достать из загашника bluetooth колонку, но это не то. Да и движений многовато, а я ленив. 
Вообщем, пробежал я как-то по нашему блошиному рынку и за какие-то копейки прихватил дохлый трехпрограммник Электроника-205. Люблю я старую советскую, что уж там скрывать, местами убогую, технику. Раскрутил - динамик рван (ну так он и новый так играет, что лучше бы изорвали), в БП кондюк какой-то отломан и транс болтается. В остальном - только паутиной все подернулось. Оживил БП - опа, а индикатор живой и прям точками мигает. Решено! Делаем WEB-радио с теплыми ламповыми часами, но чтоб "поделка выходного дня". Никаких печатных плат и заказов деталей с Ali, сложного ПО и пр. Только из того, что под рукой и быстро, т.е. из говна и палок.
Т.е. по уму надо БП на несколько напряжений, развязку по питанию между процессором и усилителем, драйвер на ИВЛ или хотя бы сдвиговый регистр с транзисторами на выходе. Но всё это не укладывается в концепцию. Короче, единственное, что я сделал - смотался за первым попавшися на Avito б/у динамиком подходящего размера, им оказался от штатной аудиосистемы VW Passatа какого-то. Дрянь конечно, но мне слушать радио причем врядли на серьезной громкости, к тому же усилитель класса D купленный на али на сдачу и аккустика пластиковой коробки - не располагают к головкам за деньги больше 300р.
Итак, NodeMCU в качестве центрального девайса. WiFi в берлоге - так себе: ничего, подпаиваем внешнюю антенну (+5-7 дБ если повезет), БП штатный оставляем для питания ИВЛ (24В + 3в переменка на накал), БП типа "семечки" 3.3В на питание ESP8266 (вот и развязка), "нормальный" БП 12В на усилок (до сих пор не пойму зачем). Чтобы управлять ИВЛ7-2/5 - оставляю КР145ИК1901 как есть и подпаиваю оптрончики к ее входам корректировки часы, минуты, сброс.
Будем устанавливать время "иммитацие нажатия кнопок Ч и М", полученное по NTP при включении и корректировать каждый час "кнопкой" Сброс.
С ПО возникли некоторые сложности. Хотел просто вкорячить прошивку от проекта esp-radio, добавив туда свою процедурку с часами. Но автор закрыл последние версии кода, а сам код такой, что кровь из глаз. Плюнул написал с нуля. Возможности получились такие:
- автоматический выбор наилучшей из доступных Wi-Fi сетей
- получение списков потоков из Мажора
- проигрывание последнего выбраного потока сразу по включению (10-15сек)
- проигрывание потоков с выбором штатными кнопками трехпрограммника и автоматический проброс "плохих" потоков
- голосовое информирование о выбраном потоке, ошибках и прочих действиях
- ОТА прошивки и SPIFFS из Мажора
- Ну и конечно теплые ламповые часы с интернетовской точностью
  
По итогу, получилось все не так плохо со звуком и сервисом. Гложет только одно: может зря я из говна все это делал...


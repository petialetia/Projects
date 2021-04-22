> [English version](ReadMe.md)

# Хеш-таблица

Это задача поделена на 3 части:

1) [Сравнение хеш функций](#сравнение-хеш-функций)

2) [Переводчик с английского](#переводчик-с-английского)

3) [Оптимизация хеш-таблицы](#оптимизация-хеш-таблицы)

Основной задачей работы было знакомство с хеш-таблицей, применение её в реальной программе и оптимизация с помощью моих знаний ассемблера. 
 
# Сравнение хеш функций

## Задача: изучить различные функции хеширования на предмет эффективности, т.е. равномерности заполнения таблицы и скорости работы.

Сравнивались следующие функции:

1) Унарный хеш: всегда возвращает единицу

![](ComparisonOfHashFunctions/Graphics/CountUnaryHash.png)

График распределения:

![](ComparisonOfHashFunctions/Graphics/Distribution/CountUnaryHashDistribution.png)

График сильно приближен, чтобы был виден меленький пик в правом крае

2) Хеш по длине: возвращает длину слова (на графике не показаны данные по всем бакетам чтобы лучше показать форму пика)

![](ComparisonOfHashFunctions/Graphics/CountFirstAsciiHash.png)

График распределения:

![](ComparisonOfHashFunctions/Graphics/Distribution/CountFirstAsciiHashDistribution.png)

График сильно приближен, чтобы был виден не только самый большой пик, но и пики поменьше

3) Хеш по первому ASCII коду: возвращает ASCII код первого символа (на графике не показаны данные по всем бакетам чтобы лучше показать форму пика)

![](ComparisonOfHashFunctions/Graphics/CountLengthHash.png)

График распределения:

![](ComparisonOfHashFunctions/Graphics/Distribution/CountLengthHashDistribution.png)

График сильно приближен, чтобы был виден не только самый большой пик, но и пики поменьше

4) Хеш по сумме ASCII кодов: возвращает сумму ASCII кодов символов

![](ComparisonOfHashFunctions/Graphics/CountSumAsciiHash.png)

График распределения:

![](ComparisonOfHashFunctions/Graphics/Distribution/CountSumAsciiHashDistribution.png)

Первые 4 функции являются, можно сказать, псевдохешами, поскольку имеют очень много коллизий и не заполняют некоторые бакеты, что критично для хеш-таблицы.

5) Ror хеш: хеш, посчитаный с использованием команды ror

![](ComparisonOfHashFunctions/Graphics/CountRorHash.png)

График распределения:

![](ComparisonOfHashFunctions/Graphics/Distribution/CountRorHashDistribution.png)

6) Rol хеш: хеш, посчитаный с использованием команды rol

![](ComparisonOfHashFunctions/Graphics/CountRolHash.png)

График распределения:

![](ComparisonOfHashFunctions/Graphics/Distribution/CountRolHashDistribution.png)

Предыдущие 2 функции, казалось бы, крайне похожи, однако распределение у Rol хеша гораздо лучше, да и работает он быстрее. 

![](ComparisonOfHashFunctions/Results/RorVsRolTime.png)

Связано это с тем, что при использовании Ror хеша мы куда чаще берём остаток от деления, ведь мы постоянно делаем циклическим сдвиг вправо, то есть у нас с высокой вероятностью старшие биты ненулевые, а потому число большое. Из-за этого увеличивается время работы и увеличивается кол-во коллизий. В этом плане Ror хеш полность проигрывает Rol хешу.

7) Полиноминальный хеш: обычный полиноминальный хеш

![](ComparisonOfHashFunctions/Graphics/CountPolynomialHash.png)

График распределения:

![](ComparisonOfHashFunctions/Graphics/Distribution/CountPolynomialHashDistribution.png)

Полиноминальный хеш работает примерно в 1,5 раза дольше своего главного соперника - Rol хеша, но при этом распределение гораздо лучше, а показатель дисперсии в целых 2 раза меньше. Поэтому мы заключаем, что в данном сравнении наиболее выгодное положение имеет CountPolynomialHash, который мы и будем использовать в дальнейшем.

## Тесты по времени работы

![](ComparisonOfHashFunctions/Results/Runtime.png)

## Дисперсия

![](ComparisonOfHashFunctions/Results/Dispersion.png)

# Переводчик с английского

## Задача: воспользоваться хеш-таблицей на практике, создав переводчик текстов на основе словаря. 

Программа анализирует текс из Text.txt и создаёт файл Translated.html, который можно открыть в браузере. При наведении на слово, если оно было переводо, будет высвечена вырезка из переводчика, иначе будет выведено сообщение "ERROR". 

### Описание работы программы:

1) Берём слово, пытаемся найти его в словаре, если находим, то пишем его перевод

2) Рассматриваем текущее и следующее слова как единую конструкцию, пытаемся найти их в словаре вместе, если находим, то пишем их перевод

3) Забываем про следующее слово и пытаемся укоротить текущее, сначала на 1 букву, затем на 2. Это позволяет перевести многие словоформы, такие как множественное число, прошедшее время и некоторые другие, пусть и не для всех случаев. Слово "happier" так и останется непереведённым, а вот слово "begun" будет переведено как "молить". К несчастью это те издержки, которых не избежать, если мы не хотим переусложнить задачу.

Также есть подсчёт длины текщей строки, что не позволяет делать строки слишком длинными и трудновоспринимаемыми. Перенос на следующую строку происходит корректно, даже если в изначальном тексте его не было.

# Оптимизация хеш-таблицы

### Задача: взять за основу переводчик, выбрать достаточно большой текст (в моём случае "Книга джунглей"), чтобы программа работала достаточно долго и мы могли собрать достоверную статистику по тому, какие функции больше всего замедляют программу, после чего оптимизировать их.

### ВАЖНО: передо мной стояла задача оптимизации работы именно хеш-таблицы, а не всего переводчика в целом, поэтому функции, которые работали долго, но не имели никакого отношению к хеш-таблице (к таким функциям, к примеру, относятся те, что отвечают за перевод), игнорировались, а все вызовы fprintf были закомментированы, чтобы они не занимали время. А ещё, чтобы больше времени занимали функции, работающие с хеш-таблицей, поиск повторяется 100 раз (по одному и тому же сценарию), переводчик продолжит работать корректно, а вот обращений к таблице станет 100 крат больше.

Оптимизация прошла в 6 этапов:

0) Тестируем на скорость изначальную версию 

![](HashTableOptimisation/EnglishTranslator/Screenshots/BeforeOptimisation.png)

Замечаем, что самой долгой функцей является FindBucket. И сразу после неё по времени работы идёт компаратор, который из себя представляет вызов strcmp. Было бы очень логично переписать хеш-таблицу на векторные инструкции, чтобы компаратор мог за 1 такт сравнить 2 слова, которые даны ему в виде 32-байтных векторов __m256i. Это сильно должно ускорить работу программы. Поскольку я всё равно собираюсь это сделать, первее переписывать FindBucket смысла не имеет, иначе потом придётся ещё раз переписывать FindBucket под работу с векторами.

1) Переработка хеш-таблицы под хранение ключа в виде вектора типа __m256i.

Изменение времени работы:

|                                | Average runtime |
|--------------------------------|-----------------|
| BeforeOptimisation             |    1.0223664    |
| VectorInstructionsOptimisation |    1.0071754    |

#### Процентный рост скорости: 1,49 %

![](HashTableOptimisation/EnglishTranslator/Screenshots/VectorInstructionsOptimisation.png)

Время работы незначительно уменьшилось, одного этого точно не хватит, поэтому перепишем FindBucket

2) Переписывание FindBucket на ассемблере в виде отдельного файла FindBucket.asm:

```

global FindBucket
section .text

extern CastStringToVector

FindBucket:

    push r12
    push r13
    push r14

    mov r12, rdi       ; r12 = bucket pointer
    mov r14, rdx       ; r14 = Comparator

    mov rdi, rsi
    call CastStringToVector

    vmovdqa ymm2, ymm0 ; saving vector

    mov r13, [r12 + 8] ; r13 = size of bucket

    mov r12, [r12]     ; pointer to current node

FindBucketLoop:

    test r13, r13      ; check if list is ended

    jz NothingFound

    vmovdqa ymm0, [r12]
    vmovdqa ymm1, ymm2

    call r14           ; call Comparator

    cmp eax, -1
    je WordFound

    dec r13            ; reducing num of nodes remain in bucket
    add r12, 64        ; 64 is size of node in bucket

    jmp FindBucketLoop

WordFound: 

    add r12, 32
    mov rax, [r12]     ; rax = rus translation

    jmp FindBucketExit

NothingFound:

    xor rax, rax       ; rax = nullptr

FindBucketExit:

    pop r14
    pop r13
    pop r12

    ret

```

Изменение времени работы:

|                                | Average runtime |
|--------------------------------|-----------------|
| VectorInstructionsOptimisation |    1.0071754    |
| FindBucketOptimised            |    0.8510366    |

#### Процентный рост скорости: 15,5 %

Имеем достаточно значительное увеличение производительности. Уже неплохо.

![](HashTableOptimisation/EnglishTranslator/Screenshots/FindBucketOptimised.png)

Здесь функция 0x0000000000002e20 - это FindBucket

Теперь перепишем компаратор

3) Переписываю компаратор в отдельном файле: 

![](HashTableOptimisation/EnglishTranslator/Screenshots/ComparatorOnAsm.png)

Изменение времени работы:

|                                | Average runtime |
|--------------------------------|-----------------|
| FindBucketOptimised            |    0.8510366    |
| StandartComparatorOptimised    |    0.5281352    |

#### Процентный рост скорости: 37,94 %

![](HashTableOptimisation/EnglishTranslator/Screenshots/StandartComparatorOptimised.png)

Здесь функция 0x0000000000002e20 - это StandartComparator, 0x0000000000002dc0 - FindBucket

Поскольку компаратор часто вызывается, имеет смысл "заинлайнить" его внутри функции FindBucket

4) Инлайним StandartComparator:

Прикрепляю кусок кода FindBucket, где синим выделены строчки, которые были изменены

![](HashTableOptimisation/EnglishTranslator/Screenshots/FindBucketWithComparatorAsm.png)

Изменение времени работы:

|                                | Average runtime |
|--------------------------------|-----------------|
| StandartComparatorOptimised    |    0.5281352    |
| ComparatorInligned             |    0.4290328    |

#### Процентный рост скорости: 18,76 %

Имеем значительный выхлоп только за счёт того, что избавились от многократных вызовов небольшой функции.

![](HashTableOptimisation/EnglishTranslator/Screenshots/StandartComparatorInligned.png)

Здесь функция 0x0000000000002e20 - это FindBucket

Следующая функция на очереди CountPolynomialHash

5) Переписывание CountPolynomialHash на ассемблере в виде ассемблерной вставки

Вот как я переписал функцию хеширования;

![](HashTableOptimisation/EnglishTranslator/Screenshots/PolynomialHashOptimised.png)

Изменение времени работы:

|                                | Average runtime |
|--------------------------------|-----------------|
| ComparatorInligned             |    0.4290328    |
| CountPolynomialHashOptimised   |    0.3381752    |

#### Процентный рост скорости: 21,18 %

![](HashTableOptimisation/EnglishTranslator/Screenshots/CountPolynomialHashOptimised.png)

Следующая функция это FindHashTable, однако уже сейчас видно, что она занимает достаточно мало времени в процентном отношении, поэтому вряд ли её переписывание даст значительный прирост, но ради чистоты эксперимента мы пойдём на это.

6) Переписывание FindHashTable на ассемблере

Вот её код на асме

```

global FindHashTable
section .text

extern FindBucket

FindHashTable:

    push r12
    push r13
    push r14
    mov r12, rdi          ;r12 = hash_table pointer  
    mov r13, rsi          ;r13 = key

    mov r14, 100          ;r14 = num_of_repeat
    
FindHashTableLoop:

    test r14, r14
    jz FindHashTableExit
    
    mov rdi, r13          ;rdi = key
    mov rsi, [r12 + 2 * 8];rsi = CountHash

    call rsi

    xor rdx, rdx          ;needed for division
    mov rsi, [r12 + 8]    ;rsi = length_of_hash_table

    div rsi

    mov rdi, [r12]        ;rdi = columns pointer

    mov rax, rdx          ;rdx = 3 * rdx (3 == num of 8-byte fields in node)
    add rdx, rdx          
    add rdx, rax

    shl rdx, 3            ;rdx = 8 * rdx
    add rdi, rdx          ;rdi = needed column pointer

    mov rsi, r13          ;rsi = key

    call FindBucket
    
    dec r14
    jmp FindHashTableLoop

FindHashTableExit:
    
    pop r14
    pop r13
    pop r12
    ret
    
```

Изменение времени работы:

|                                | Average runtime |
|--------------------------------|-----------------|
| CountPolynomialHashOptimised   |    0.3381752    |
| FindHashTableOptimised         |    0.3367566    |

#### Процентный рост скорости: 0,42 %

![](HashTableOptimisation/EnglishTranslator/Screenshots/FindHashTableOptimised.png)

Здесь функция 0x0000000000002d50 - это FindBucket, 0x0000000000002db0 - FindHashTable

Функция теперь занимает меньше времени в процентном отношении, однако на деле ускорение едва ли ощутимо, мы выиграли всего 0,42 %. Поэтому заключаем, что дальнейшая оптимизация не имеет особого смысла, мы потратим непропорционально много усилий на то, чтобы получить околонулевой выигрыш.

Итого мы ускорили функцию на 67% от исходного времени. Мои усилия того стоили.


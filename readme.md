# OpenGL Pong

## Виконання вимог:
1. Начебто тут все очевидно, але як доказ що це саме С++ - жодного разу за час розробки програма не вилітала через Segmentation Fault  
2. Більшість функціоналу реалізована через класи - шар, платформи, поле, шейдери, вікно програми (щоправда, клас статичний) і тд.
3. Чисті вказівники Сі-стайл в програмі не використовуються майже зовсім, хіба що це зумовлено використонаю бібліотекою (наприклад, GLFW window *)
4. Начебто код виглядає нормально 
5. Не існує
6. Окрім стандартної бібліотеки мною було використано header-only бібліотеку для роботи з .ini файлами, для роботи з графікою - GLEW & GLFW, для звуку - irrKlang.
7. Власне це розділення видно у main() функції - апдейт ігрової логіки та відмальовка розділені в різні методи класу Field.
За бажання, теоретично, оновлення можна було б виокремити в окремий потік та виконувати паралельно 
8. Для створення платформ використовується паттерн Фабрики (файл PaddleFabrics.hpp). Для створення кожної платформи використовується окрема фабрика, тобто поле не має уявлення який саме тип платформи воно створює (це може бути платформа під керівництвом ШІ, або трошки дурнуватого ШІ, або гравця). Тип фабрики задається за створення поля. 
9. Тут начебто все Ок, хіба що можуть бути проблеми з генерацією файлів для VS через використання CMake. 
10. Ви, власне, його й прочитали)

## Деякі особливості реалізації, рекомендації та пояснення

1. Деякі налаштування для програми задаються у config.ini файлі. У тому числі - дизайн поля, м'яча, тобто, загалом - дизайн гри. У фінальній версії включено 4 теми оформлення, кожна з яких пропонує абсолютно унікальний візуал. Змінити дизайн можна розкоментувавши відповідний рядок у config файлі.
2. За замовчуванням Вам пропонується зіграти проти дурнуватого бота - швидкість руху його платформи у 2.5 рази менша за стандартну. Якщо хочете справжнього виклику - замініть у створенні поля EasyAIPaddle на просто AIPaddle. А найвидовишніша гра виходить, коли між собою грають 2 сильних боти - для цього необхідно щоб за створення поля обидві фабрики були відповідного типу, як тут:


    Field field(
            std::make_shared<Ball>(Texture::CreateTexture(ballTexturePath)),

            std::make_shared<CustomPaddleFactory<AIPaddle>>(),
            std::make_shared<CustomPaddleFactory<AIPaddle>>(),

            Texture::CreateTexture(leftPaddleTexturePath),
            Texture::CreateTexture(rightPaddleTexturePath),
            Texture::CreateTexture(backgroundTexturePath)
    );
3. Через те, що для компілятора MinGW не існує (принаймні, я не знайшов) файлів бібліотеки irrKlang, що відповідає за відтворення звуків, під час компіляції програми цим компілятором усі функції пов'язані з роботою з цією бібліотекою виключаються задля запобігання помилки лінкування. Додатково, можуть виникати проблеми з відсутністю певних .dll, але основні dll зовнішніх бібліотек було включено до проекту.  

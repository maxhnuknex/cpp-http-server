# C++ HTTP Server

Учебный HTTP-сервер на C++17, написанный для практического изучения backend-разработки и внутреннего устройства web-серверов.

Цель проекта — разобраться, как сервер работает на низком уровне: как принимается TCP-соединение, как читается сырой HTTP-запрос, как он преобразуется в объект запроса, проходит через middleware, попадает в router и превращается в HTTP-ответ для клиента.

Проект не использует готовые web-фреймворки. Основные части сервера реализуются вручную, чтобы лучше понять архитектуру backend-систем.

## Что реализовано

* TCP-сервер на POSIX sockets
* Приём клиентских подключений
* Парсинг HTTP-запроса
* Класс `HTTPRequest` для хранения метода, пути, версии, заголовков и тела запроса
* Класс `HTTPResponse` для формирования HTTP-ответа
* Router для выбора обработчика по HTTP-методу и пути
* Static file handler для отдачи файлов из папки `public`
* Middleware pipeline для промежуточной обработки запроса
* ThreadPool для многопоточной обработки клиентских подключений
* Сборка проекта через CMake
* Разделение кода на модули `HTTP` и `TCPserver`

## Идея проекта

Обычно backend-разработка начинается с готовых фреймворков, где большая часть внутренней логики уже скрыта. В этом проекте я реализую базовые механизмы сервера самостоятельно, чтобы понимать, что происходит внутри backend-приложения.

Проект показывает полный путь запроса:

text
Client
  ↓
TCPServer
  ↓
HTTPParser
  ↓
HTTPRequest
  ↓
Middleware Pipeline
  ↓
Router
  ↓
Handler / StaticFileHandler
  ↓
HTTPResponse
  ↓
Client


## Архитектура

Проект построен по принципу разделения ответственности. Каждый компонент отвечает только за свою часть работы.

`TCPServer` отвечает за сетевой уровень: создание socket, bind, listen, accept, чтение запроса от клиента и отправку ответа.

`HTTPParser` отвечает за преобразование сырого HTTP-запроса в структурированный объект `HTTPRequest`.

`HTTPRequest` хранит данные HTTP-запроса: метод, путь, версию протокола, заголовки и тело.

`HTTPResponse` формирует HTTP-ответ: статус, заголовки и тело ответа.

`Router` отвечает за маршрутизацию. Он выбирает нужный handler по методу и пути запроса.

`StaticFileHandler` отвечает за отдачу статических файлов из папки `public`. Например, HTML и CSS.

`Pipeline` отвечает за middleware-цепочку. Middleware позволяет добавить промежуточную обработку запроса до вызова основного handler.

`ThreadPool` отвечает за выполнение клиентских запросов в нескольких потоках через очередь задач.

Такое разделение делает проект более понятным, расширяемым и удобным для дальнейшего развития.

## Структура проекта

text
include/
  HTTP/
    HTTPRequest.h
    HTTPResponse.h
    HTTPParser.h
    Router.h
    StaticFileHandler.h
    Pipeline.h

  TCPserver/
    tcpserver.h
    ThreadPool.h

src/
  HTTP/
    HTTPResponse.cpp
    HTTPParser.cpp
    Router.cpp
    StaticFileHandler.cpp
    Pipeline.cpp

  TCPserver/
    TCPserver.cpp
    ThreadPool.cpp

  main.cpp

public/
  index.html
  style.css

CMakeLists.txt
README.md


## Сборка

cmake -build build


## Запуск

bash
./server


После запуска сервер доступен по адресу:

http://localhost:8080


## Пример запроса

curl http://localhost:8080/


Также можно открыть в браузере:

http://localhost:8080/

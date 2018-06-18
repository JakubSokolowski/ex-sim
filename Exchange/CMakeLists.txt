project(OrderBook)

set(HEADER_FILES
        Order.h
        PricePoint.h
        OrderBook.h
        StockExchange.h
        Player.h
        Coordinator.h
        MatchingEngine.h
        SafeQueue.h
        EventGenerator.h
        ExchangeChannel.h
        RandUtil.h
        Printer.h
        Visualisation.h
        )
set(SOURCE_FILES
        Order.cpp
        OrderBook.cpp
        PricePoint.cpp
        StockExchange.cpp
        Player.cpp
        Coordinator.cpp
        MatchingEngine.cpp
        EventGenerator.cpp
        ExchangeChannel.cpp
        Printer.cpp
        Visualisation.cpp
        )


add_library(Exchange STATIC ${SOURCE_FILES} ${HEADER_FILES})
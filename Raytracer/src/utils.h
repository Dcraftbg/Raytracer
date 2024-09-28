#pragma 
#define DEL_MOVE(class) \
    private:\
        class(class&&) = delete;\
        class& operator=(class&&) = delete;

#define DEL_COPY(class) \
    private:\
        class(const class&) = delete;\
        class& operator=(const class&) = delete;


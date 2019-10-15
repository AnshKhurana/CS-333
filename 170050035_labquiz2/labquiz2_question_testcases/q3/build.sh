g++ test-zem.c zemaphore.c -o test-zem -lpthread
g++ test-toggle.c zemaphore.c -o test-toggle -lpthread
g++ test-rwlock.cpp rwlock.c zemaphore.c -o rwlock -lpthread

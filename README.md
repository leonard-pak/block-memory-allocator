# Block Memory Allocator

## Tasks

- [ ] Allocate memory one block at a time of fixed size from static pool
- [ ] The block and pool size must be configurable at project build time
- [ ] Adapted to work on Embedded platform of different sizes in a RTOS
- [ ] Unit-tests
- [ ] Only english

### 1. Allocate memory one block at a time of fixed size from static pool

TODO:
1. Инициализации памяти
2. Выделения памяти
3. Освобождения памяти

### 2. The block and pool size must be configurable at project build time

TODO:
1. Сделать через макросы и добавить флаги в систему сборки

### 3. Adapted to work on Embedded platform of different sizes in a RTOS

TODO:
1. Добавить проверку на этапе компияции размерности системы. Подготовить тип блока в соответствии с его размером, если блок укладываетсяв размер системы.
2. Добавить при инициализации интрефейс для прокидывания управлением мьютексом, чтобы предостеречь конкурентный доступ.

### 4. Unit-tests

TODO GTest:
1. Проверить аллокацию и высвобождение памяти

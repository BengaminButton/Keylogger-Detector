<div align="center">

# KeyShield — Антикейлоггер (Qt6, C++20)

<img width="1875" height="997" alt="изображение" src="https://github.com/user-attachments/assets/20230778-ff08-485b-97d7-ce7c859c6444" />


Современный кроссплатформенный антикейлоггер с мягким тёмным GUI. Есть тест «Захват» для проверки ввода, автозагрузка и задел для детекторов Windows/Linux и YARA.

</div>

---

## Возможности
- Мягкий тёмный интерфейс (Qt6, крупные элементы)
- Кнопки: «Сканировать», «Защита ВКЛ/ВЫКЛ», «Захват ВКЛ/ВЫКЛ», «В автозапуск», «Убрать автозапуск»
- Журнал событий: результаты сканирования и тест ввода
- Абстракция детекторов (Linux/Windows) + фабрика [createDetector()](cci:1://file:///run/media/thm/Disk_G/AntiKeyLogger/include/detector.h:20:0-20:43)
- YARA — опционально (сборка проходит и без неё)

## Дорожная карта
- Linux: поиск чтения `/dev/input/event*`, unlinked бинарники в `/proc/<pid>/exe`, аномалии `cmdline`
- Windows: WH_KEYBOARD_LL, анализ процессов (Toolhelp/WMI), интеграция YARA

## Сборка
```bash
cmake -S . -B build
cmake --build build -j

Автор: t.me/Bengamin_Button

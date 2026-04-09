C++ Pong Game (SFML 3.0)

Керування:

	Стрілки Up / Down або клавіші W / S для руху.
	Enter для перезапуску гри після фіналу.
	Esc для виходу.


Виконання вимог

	1. Знання C++ (Modern):						Використано стандарти C++17/20: std::optional, enum class, списки ініціалізації.
	2. Використання ООП:						Класи Entity, Paddle, Ball та AIController. Логіка інкапсульована в об'єкти.
	3. STL (Контейнери та Розумні вказівники):	Всюди використано std::unique_ptr для автоматичного керування пам'яттю. Звуки та декорації зберігаються в std::vector.
	4. Акуратний код:							Код відформатований згідно зі стандартами, має чітку структуру та коментарі.
	5. Зовнішні бібліотеки:						Використано SFML 3.0 для графіки (Graphics), звуку (Audio) та роботи з вікном.
	6. Архітектура рушія:						Ігровий цикл (Game::run) розділено на етапи: processEvents, update та render.
	7.Патерни проектування:						Використано патерн Controller (AI) та State (логіка завершення гри).
	8.Портативність (F5 запуск):				CMake налаштовано на автоматичне копіювання папок assets та .dll (на Windows) у папку білду.

Покращення та Бонуси (Extra)

	1.Звукова система:							Додано звуки удару, голів та фіналу.

Можна було реалізувати патерн factory наступним чином:
```
    enum class EntityType { PlayerPaddle, AIPaddle, StandardBall, SpeedBall };

    class EntityFactory {
    public:
        static std::unique_ptr<Entity> createEntity(EntityType type, float x, float y) {
            switch (type) {
                case EntityType::PlayerPaddle:
                    return std::make_unique<Paddle>(x, y, true); // true - керований гравцем
                case EntityType::AIPaddle:
                    return std::make_unique<Paddle>(x, y, false); // false - бот
                case EntityType::StandardBall:
                    return std::make_unique<Ball>(x, y, 300.f); // звичайна швидкість
                case EntityType::SpeedBall:
                    return std::make_unique<Ball>(x, y, 600.f); // подвоєна швидкість
                default:
                    return nullptr;
            }
        }
    };
```
для динамічного створення різних типів ігрових сутностей. Але я сильний прихильник концепьу KISS, тому реалізована фіксована кількість об'єктів.
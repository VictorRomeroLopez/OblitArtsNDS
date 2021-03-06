enum class GameStateType
{
    Start,
    Playing,
    End
};

class GameState
{
    public:
        virtual void OnEnter() = 0;
        virtual void Update() = 0;
        virtual void OnExit() = 0;
};

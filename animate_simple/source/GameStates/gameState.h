enum class GameStateType{
    Start,
    Playing,
    End
};

class IState{
    public:
        IState(){}
        virtual void OnEnter() = 0;
        virtual void Update() = 0;
        virtual void OnExit() = 0;
};

class GameState : public IState{
    public:
        virtual void OnEnter() = 0;
        virtual void Update() = 0;
        virtual void OnExit() = 0;
};

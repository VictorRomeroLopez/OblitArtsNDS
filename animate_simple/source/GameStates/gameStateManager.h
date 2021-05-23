#include <map>

class GameState;
enum GameStateType;
class GameStarting;
class Game;

class GameStateManager
{
    public:
        std::map<GameStateType, GameState*> stateTypeToGameState;
        GameStateType currentState;

        GameStateManager(Game* game)
        {
            stateTypeToGameState[GameStateType::Start] = new GameStarting(game);
            stateTypeToGameState[GameStateType::Playing] = new GamePlaying(game);
            stateTypeToGameState[GameStateType::End] = new GameEnd(game);
            currentState = game->currentState;
            stateTypeToGameState[currentState]->OnEnter();
        }

        ~GameStateManager()
        {
            for(std::pair<GameStateType, GameState*> state : stateTypeToGameState){
                delete[] state.second;
            }
        }

        void Update()
        {
            stateTypeToGameState[currentState]->Update();
        }

        void ChangeState(GameStateType type)
        {
            stateTypeToGameState[currentState]->OnExit();
            currentState = type;
            stateTypeToGameState[currentState]->OnEnter();
        }
};
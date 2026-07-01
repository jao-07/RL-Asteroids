import gymnasium as gym
import numpy as np
import asteroids_cpp

class AsteroidsEnv(gym.Env):

    def __init__(self, 
                 render_mode="none", 
                 allowSplitAsteroids = False, 
                 asteroidsNumber = 10, 
                 asteroidDestroyedReward = 1.0, 
                 loseReward = -20.0, 
                 winReward = 10.0, 
                 laserMissReward = -0.05, 
                 timeReward = 0.0, 
                 proximityAndDirectionReward = 0.0
                 ):
        super().__init__()
        self.render_mode = render_mode
        self.allowSplitAsteroids = allowSplitAsteroids
        self.asteroidsNumber = asteroidsNumber
        self.asteroidDestroyedReward = asteroidDestroyedReward
        self.loseReward = loseReward
        self.winReward = winReward
        self.laserMissReward = laserMissReward
        self.timeReward = timeReward
        self.proximityAndDirectionReward = proximityAndDirectionReward

        visualize = (self.render_mode == "human")
        self.game = asteroids_cpp.Game(visualize, 
                                       allowSplitAsteroids,
                                       asteroidsNumber,
                                       asteroidDestroyedReward,
                                       loseReward,
                                       winReward,
                                       laserMissReward,
                                       timeReward,
                                       proximityAndDirectionReward
                                       )
        success = self.game.initialize()
        if not success:
            print("Error in the SDL Window creation!")
            exit()
        
        # Ações Discretas(0: Dir, 1: Esq, 2: Tiro, 3: Frente, 4: Nada)
        self.action_space = gym.spaces.Discrete(5)

        self.observation_space = gym.spaces.Box(
            low=-1, 
            high=1,
            shape=(21,),
            dtype=np.float32
        )

    def reset(self, seed=None, options=None):
        super().reset(seed=seed)
        
        state = self.game.reset()
        info = {}
        
        return state, info

    def step(self, action):
        state_cpp, reward, terminated, truncated, stats = self.game.step(int(action))
        
        state = np.array(state_cpp, dtype=np.float32)
        info = {}
        if stats[0]:
            info["episode_stats"] = {
                "shotsFired": stats[1],
                "shotsHit": stats[2],
                "survivalTime": stats[3],
                "victory": stats[4],
                "accuracy": stats[5]
            }
        
        return state, float(reward), bool(terminated), bool(truncated), info

    def render(self):
        pass

    def close(self):
        self.game.shutdown()
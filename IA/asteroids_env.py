import gymnasium as gym
import numpy as np
import asteroids_cpp

class AsteroidsEnv(gym.Env):

    def __init__(self, 
                 render_mode="none", 
                 allowSplitAsteroids = False,
                 widthImage = 128,
                 heightImage = 128,
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
        self.widthImage = widthImage
        self.heightImage = heightImage
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
            low=0,
            high=255,
            shape=(128,128),
            dtype=np.uint8
        )

    def _get_obs(self):
        raw_pixels = self.game.get_screen_pixels(self.widthImage, self.heightImage) 
        obs = np.array(raw_pixels, dtype=np.uint8).reshape((self.widthImage, self.heightImage))
        return obs

    def reset(self, seed=None, options=None):
        super().reset(seed=seed)
        
        self.game.reset()
        obs = self._get_obs()
        info = {}
        
        return obs, info

    def step(self, action):
        reward, terminated, truncated, stats = self.game.step(int(action))
        obs = self._get_obs()
        
        info = {}
        if stats[0]:
            info["episode_stats"] = {
                "shotsFired": stats[1],
                "shotsHit": stats[2],
                "survivalTime": stats[3],
                "victory": stats[4],
                "accuracy": stats[5]
            }
        
        return obs, float(reward), bool(terminated), bool(truncated), info

    def render(self):
        pass

    def close(self):
        self.game.shutdown()

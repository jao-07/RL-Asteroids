import asteroids_cpp
import os
from pathlib import Path
from stable_baselines3 import PPO
from stable_baselines3.common.callbacks import CallbackList, CheckpointCallback, BaseCallback
from stable_baselines3.common.vec_env import SubprocVecEnv
from stable_baselines3.common.monitor import Monitor
from asteroids_env import AsteroidsEnv
from gymnasium.wrappers import FrameStackObservation

class CustomTensorboardCallback(BaseCallback):

    def _on_step(self):
        for info in self.locals["infos"]:
            if "episode_stats" in info:
                stats = info["episode_stats"]
                self.logger.record(
                    "custom/shotsFired",
                    stats["shotsFired"]
                )
                self.logger.record(
                    "custom/shotsHit",
                    stats["shotsHit"]
                )
                self.logger.record(
                    "custom/survivalTime",
                    stats["survivalTime"]
                )
                self.logger.record(
                    "custom/victory",
                    stats["victory"]
                )
                self.logger.record(
                    "custom/accuracy",
                    stats["accuracy"]
                )
        return True

def make_env(rank, stack_size=4):
    def _init():
        env = AsteroidsEnv(
            render_mode="none",
            asteroidDestroyedReward = 1.0,
            loseReward = -1.0,
            winReward = 1.0,
            laserMissReward = 0.0,
        )
        env = Monitor(env)
        env = FrameStackObservation(env, stack_size=stack_size)
        return env
    return _init

if __name__ == "__main__":

    num_envs = 4
    env = SubprocVecEnv([make_env(i) for i in range(num_envs)])

    OS_CHECKPOINT_DIR = "Checkpoints/checkpoints_cnn_teste_cpu"
    os.makedirs(OS_CHECKPOINT_DIR, exist_ok=True)

    checkpoint_callback = CheckpointCallback(
        save_freq=100000,
        save_path=OS_CHECKPOINT_DIR,
        name_prefix="2M_84px",
        save_replay_buffer=False,
        save_vecnormalize=False,
        verbose=1
    )

    tensorboard_callback = CustomTensorboardCallback()

    callback = CallbackList([
        checkpoint_callback,
        tensorboard_callback
    ])

    # modelo_ppo = PPO(
    #     "CnnPolicy",
    #     env,
    #     learning_rate=1e-4,
    #     n_steps=2048,
    #     batch_size=256,
    #     gamma=0.99,
    #     gae_lambda=0.95,
    #     ent_coef=0.01,
    #     verbose=1,
    #     tensorboard_log="./tensorBoardFiles/"
    # )
    OS_MODEL_DIR = "models/CNN_6M_84px.zip"
    modelo_ppo = PPO.load(
        OS_MODEL_DIR,
        env=env,
        custom_objects={
            "n_steps": 1024,
            "batch_size": 512,
            "n_epochs": 4,
            "ent_coef": 0.0001
        }
    )

    modelo_ppo.learn(
        total_timesteps=3000000,
        tb_log_name="CNN_2M_84px",
        callback=callback,
        reset_num_timesteps=False,
        progress_bar=True
    )

    MODELS_DIR = Path("models")
    MODELS_DIR.mkdir(parents=True, exist_ok=True)
    path = MODELS_DIR / "CNN_9M_84px"
    modelo_ppo.save(path)

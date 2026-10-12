import asteroids_cpp
import os
import torch
from pathlib import Path
from stable_baselines3 import PPO
from stable_baselines3.common.callbacks import CallbackList, CheckpointCallback, BaseCallback
from stable_baselines3.common.vec_env import SubprocVecEnv
from stable_baselines3.common.monitor import Monitor
from asteroids_env import AsteroidsEnv
from gymnasium.wrappers import FrameStackObservation

os.environ["OMP_NUM_THREADS"] = "2"
os.environ["MKL_NUM_THREADS"] = "2"
torch.set_num_threads(2)

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
        env = AsteroidsEnv(render_mode="none")
        env = Monitor(env)
        env = FrameStackObservation(env, stack_size=stack_size)
        return env
    return _init

def linear_schedule(initial_value: float):
    def func(progress_remaining: float) -> float:
        return progress_remaining * initial_value
    return func

if __name__ == "__main__":

    num_envs = 4
    env = SubprocVecEnv([make_env(i) for i in range(num_envs)])

    OS_CHECKPOINT_DIR = "Checkpoints/CNN_84px"
    os.makedirs(OS_CHECKPOINT_DIR, exist_ok=True)

    checkpoint_callback = CheckpointCallback(
        save_freq=max(10000, 100000 // num_envs),
        save_path=OS_CHECKPOINT_DIR,
        name_prefix="CNN_84px",
        save_replay_buffer=False,
        save_vecnormalize=False,
        verbose=1
    )

    tensorboard_callback = CustomTensorboardCallback()

    callback = CallbackList([
        checkpoint_callback,
        tensorboard_callback
    ])

    modelo_ppo = PPO(
        "CnnPolicy",
        env,
        learning_rate=linear_schedule(1e-4),
        n_steps=2048,
        batch_size=256,
        n_epochs=4,
        gamma=0.99,
        gae_lambda=0.95,
        ent_coef=0.0005,
        verbose=1,
        tensorboard_log="./tensorBoardFiles/"
    )
    OS_MODEL_DIR = "models/CNN_4M_84px.zip"
    # modelo_ppo = PPO.load(
    #     OS_MODEL_DIR,
    #     env=env,
    #     device="cpu",
    # )

    modelo_ppo.learn(
        total_timesteps=6000000,
        tb_log_name="CNN_84px",
        callback=callback,
        reset_num_timesteps=False,
        progress_bar=True
    )

    MODELS_DIR = Path("models")
    MODELS_DIR.mkdir(parents=True, exist_ok=True)
    path = MODELS_DIR / "CNN_6M_84px"
    modelo_ppo.save(path)

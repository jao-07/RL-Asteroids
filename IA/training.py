import asteroids_cpp
import os
from pathlib import Path
from stable_baselines3 import PPO
from stable_baselines3.common.callbacks import CallbackList, CheckpointCallback, BaseCallback
from stable_baselines3.common.vec_env import SubprocVecEnv
from stable_baselines3.common.monitor import Monitor
from asteroids_env import AsteroidsEnv

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

def make_env(rank, seed=0):
    def _init():
        env = AsteroidsEnv(render_mode="none")
        return Monitor(env)
    return _init

if __name__ == "__main__":

    num_envs = 4
    env = SubprocVecEnv([make_env(i) for i in range(num_envs)])

    OS_CHECKPOINT_DIR = "Checkpoints/checkpoints_ppo_10ast_2obs"
    os.makedirs(OS_CHECKPOINT_DIR, exist_ok=True)

    checkpoint_callback = CheckpointCallback(
        save_freq=max(10000, 200000 // num_envs),
        save_path=OS_CHECKPOINT_DIR,
        name_prefix="teste",
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
        "MlpPolicy",
        env,
        learning_rate=3e-4,
        n_steps=2048,
        batch_size=256,
        gamma=0.99,
        gae_lambda=0.95,
        ent_coef=0.005,
        policy_kwargs=dict(net_arch=dict(
            pi=[128, 128],
            vf=[128, 128])
        ),
        verbose=1,
        tensorboard_log="./tensorBoardFiles/"
    )
    # modelo_ppo = PPO.load("ppo_10ast_2obs_2", env=env_diff1)

    modelo_ppo.learn(
        total_timesteps=1000000,
        tb_log_name="teste",
        callback=callback,
        reset_num_timesteps=False,
        progress_bar=True
    )

    MODELS_DIR = Path("models")
    MODELS_DIR.mkdir(parents=True, exist_ok=True)
    path = MODELS_DIR / "model_10ast"
    modelo_ppo.save(path)

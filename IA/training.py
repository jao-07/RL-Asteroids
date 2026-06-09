import asteroids_cpp
import os
from stable_baselines3 import PPO
from stable_baselines3.common.callbacks import CheckpointCallback
from stable_baselines3.common.vec_env import SubprocVecEnv
from stable_baselines3.common.vec_env import DummyVecEnv
from stable_baselines3.common.monitor import Monitor
from asteroids_env import AsteroidsEnv

def make_env(rank, seed=0):
    def _init():
        env = AsteroidsEnv(render_mode="none", difficulty=1)
        return Monitor(env)
    return _init

if __name__ == "__main__":

    num_envs = 4
    env_diff1 = SubprocVecEnv([make_env(i) for i in range(num_envs)])
    # env_diff1 = DummyVecEnv([make_env(i) for i in range(num_envs)])

    OS_CHECKPOINT_DIR = "./checkpoints_ppo_10ast_aimRew"
    os.makedirs(OS_CHECKPOINT_DIR, exist_ok=True)

    checkpoint_callback = CheckpointCallback(
        save_freq=max(10000, 200000 // num_envs),
        save_path=OS_CHECKPOINT_DIR,
        name_prefix="ppo_10ast_aimRew_2M",
        save_replay_buffer=False,
        save_vecnormalize=False,
        verbose=1
    )

    modelo_ppo = PPO(
        "MlpPolicy",
        env_diff1,
        learning_rate=1e-4,
        n_steps=2048,
        batch_size=256,
        gamma=0.99,
        gae_lambda=0.95,
        ent_coef=0.005,
        policy_kwargs=dict(net_arch=dict(pi=[128, 256], vf=[128, 256])),
        verbose=1,
        tensorboard_log="./ppo_diff1/"
    )
    # modelo_ppo = PPO.load("ppo_10ast_5M", env=env_diff1)

    modelo_ppo.learn(
        total_timesteps=2000000,
        tb_log_name="ppo_10ast_aimRew_2M",
        callback=checkpoint_callback,
        reset_num_timesteps=False,
        progress_bar=True
    )

    modelo_ppo.save("ppo_10ast_aimRew_2M")

import asteroids_cpp
import os
from stable_baselines3 import PPO
from stable_baselines3.common.callbacks import CheckpointCallback
from stable_baselines3.common.vec_env import SubprocVecEnv
from asteroids_env import AsteroidsEnv

def make_env(rank, seed=0):
    def _init():
        env = AsteroidsEnv(render_mode="none", difficulty=1)
        return env
    return _init

if __name__ == "__main__":

    num_envs = 4
    env = SubprocVecEnv([make_env(i) for i in range(num_envs)])

    OS_CHECKPOINT_DIR = "./checkpoints_ppo_8ast/"
    os.makedirs(OS_CHECKPOINT_DIR, exist_ok=True)

    checkpoint_callback = CheckpointCallback(
        save_freq=max(10000, 200000 // num_envs),
        save_path=OS_CHECKPOINT_DIR,
        name_prefix="ppo_8asteroides",
        save_replay_buffer=False,
        save_vecnormalize=False,
        verbose=1
    )

    modelo_ppo = PPO(
        "MlpPolicy",
        env,
        learning_rate=3e-4,
        n_steps=2048,
        batch_size=128,
        gamma=0.99,
        gae_lambda=0.95,
        ent_coef=0.05,
        policy_kwargs=dict(net_arch=dict(pi=[128, 256], vf=[128, 256])),
        verbose=1,
        tensorboard_log="./ppo_diff1_8ast/"
    )

    modelo_ppo.learn(
        total_timesteps=600000,
        tb_log_name="ppo_diff1_8ast",
        callback=checkpoint_callback,
        progress_bar=True
    )

    modelo_ppo.save("ppo_diff1__8ast_final")

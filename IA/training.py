import asteroids_cpp
import os
from stable_baselines3 import PPO
from stable_baselines3.common.callbacks import CheckpointCallback
from asteroids_env import AsteroidsEnv

OS_CHECKPOINT_DIR = "./checkpoints_ppo_8ast/"
os.makedirs(OS_CHECKPOINT_DIR, exist_ok=True)

checkpoint_callback = CheckpointCallback(
    save_freq=200000,
    save_path=OS_CHECKPOINT_DIR,
    name_prefix="ppo_8asteroides",
    save_replay_buffer=False,
    save_vecnormalize=False,
    verbose=1
)

env = AsteroidsEnv(render_mode="none", difficulty=1)

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
    total_timesteps=1200000,
    tb_log_name="ppo_diff1_8ast_1.2M",
    callback=checkpoint_callback,
    progress_bar=True
)

modelo_ppo.save("ppo_diff1_200T_Prox")

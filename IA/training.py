import asteroids_cpp
from stable_baselines3 import PPO
from asteroids_env import AsteroidsEnv
import torch

torch.autograd.set_detect_anomaly(True)

env = AsteroidsEnv(render_mode="none", difficulty=1)

modelo_ppo = PPO(
    "MlpPolicy",
    env,
    learning_rate=3e-4,
    n_steps=2048,
    batch_size=64,
    gamma=0.99,
    ent_coef=0.05,
    policy_kwargs=dict(net_arch=dict(pi=[128, 256], vf=[128, 256])),
    verbose=1,
    tensorboard_log="./ppo_diff1/"
)

modelo_ppo.learn(total_timesteps=200000, tb_log_name="ppo_diff1_200mil_missRew", progress_bar=True)

modelo_ppo.save("ppo_diff1_200mil_missRew")

import asteroids_cpp
from stable_baselines3 import PPO
from asteroids_env import AsteroidsEnv

env = AsteroidsEnv(render_mode="none", survivalReward=0.1, asteroidDestroyedReward=5.0, deathReward=-50.0)

modelo_ppo = PPO(
    "MlpPolicy",
    env,
    verbose=1,
    tensorboard_log="./ppo_asteroids_tensorboard/",
    policy_kwargs=dict(net_arch=[128, 256]),
    ent_coef=0.01
)

modelo_ppo.learn(total_timesteps=500000, tb_log_name="ppo_500mil")

modelo_ppo.save("ppo_500mil")

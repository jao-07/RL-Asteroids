import asteroids_cpp
from stable_baselines3 import DQN
from asteroids_env import AsteroidsEnv

env = AsteroidsEnv(render_mode="none", survivalReward=0.1, asteroidDestroyedReward=5.0, deathReward=-50.0)

params = {'alpha': 1e-3,
          'gamma': 0.9,
          'batch_size': 128,
          'buffer_size': 50000,
          'epsilon_decay': 0.9990115359881433,
          'target_update': 500,
          'train_freq': 4,
          'episodes': 2000}

# dqn_model = DQN("MlpPolicy",
#     env,
#     verbose=1,
#     buffer_size=params['buffer_size'],
#     batch_size=params['batch_size'],
#     train_freq=params['train_freq'],
#     gamma=params["gamma"],
#     exploration_fraction=0.5,
#     exploration_initial_eps=1,
#     exploration_final_eps=0.1,
#     target_update_interval=params['target_update'],
#     learning_rate=params['alpha'],
#     tensorboard_log="./logs_tensorboard/"
#     )

new_parameters = {
    "exploration_fraction": 0.1,
    "exploration_initial_eps": 0.1,
    "exploration_final_eps": 0.01
}

dqn_model = DQN.load("dqn_asteroids_15_2400000steps", env=env, custom_objects=new_parameters)
dqn_model.learn(total_timesteps=1000000, progress_bar=True)
print("Treinamento finalizado. Salvando o modelo...")
dqn_model.save("dqn_asteroids_FT_exp_frac_1M")

# print("Executando 100 passos de simulação...")

# for i in range(100):
#     state = jogo.reset()
#     terminated = False
#     truncated = False
#     ep_reward = 0

#     while not (terminated or truncated):
#         action = random.randint(0,5)
#         next_state, reward, terminated, truncated = jogo.step(action)
#         state = next_state
#         ep_reward += reward

#     print(f"Episodio finalizado\n Recompensa final: {ep_reward}")
    
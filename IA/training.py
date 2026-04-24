import asteroids_cpp
from stable_baselines3 import DQN
from asteroids_env import AsteroidsEnv

env = AsteroidsEnv(render_mode="none", survivalReward=0.1, asteroidDestroyedReward=5.0, deathReward=-50.0)

params = {'alpha': 0.00017195082231670288,
          'gamma': 0.9778366856839303,
          'batch_size': 128,
          'buffer_size': 50000,
          'epsilon_decay': 0.9990115359881433,
          'target_update': 500,
          'train_freq': 4,
          'episodes': 2000}

modelo_dqn = DQN("MlpPolicy", 
    env, 
    verbose=1, 
    buffer_size=params['buffer_size'],
    batch_size=params['batch_size'],
    train_freq=params['train_freq'], 
    gamma=params["gamma"], 
    exploration_fraction=0.5,
    exploration_initial_eps=1,
    exploration_final_eps=0.1, 
    target_update_interval=params['target_update'], 
    learning_rate=params['alpha'],
    tensorboard_log="./logs_tensorboard/"
    )

modelo_dqn.learn(total_timesteps=200000, progress_bar=True)
print("Treinamento finalizado. Salvando o modelo...")
modelo_dqn.save("dqn_asteroids_teste")

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
    
from stable_baselines3 import PPO
from asteroids_env import AsteroidsEnv

print("Carregando o ambiente com interface gráfica...")
env = AsteroidsEnv(render_mode="human")

print("Carregando o modelo treinado...")
model = PPO.load("ppo_10ast_2obs_split")

obs, _ = env.reset()
done = False
total_reward = 0.0

print("Deixando a IA jogar...")

for i in range(20):
    while True:
        acao, _estados_internos = model.predict(obs, deterministic=True)

        obs, recompensa, done, trunc, _ = env.step(acao)

        total_reward += recompensa
        if done or trunc:
            print(f"\nFim de jogo {i}! Recompensa total da IA: {total_reward:.2f}")
            obs, _ = env.reset()
            total_reward = 0
            break

# Fecha o ambiente e destrói a janela do SDL com segurança
env.close()
from stable_baselines3 import DQN
from asteroids_env import AsteroidsEnv  # Importa a classe que criamos

print("Carregando o ambiente com interface gráfica...")
env = AsteroidsEnv(render_mode="human")

print("Carregando o modelo treinado...")
modelo = DQN.load("dqn_asteroids_teste")

obs, _ = env.reset()
terminou = False
recompensa_total = 0.0

print("Deixando a IA jogar...")

while True:
    acao, _estados_internos = modelo.predict(obs, deterministic=True)
    
    obs, recompensa, term, trunc, _ = env.step(acao)
    
    recompensa_total += recompensa
    if term or trunc:
        print(f"\nFim de jogo! Recompensa total da IA: {recompensa_total:.2f}")
        obs, _ = env.reset()
        recompensa_total = 0

print(f"\nFim de jogo! Recompensa total da IA: {recompensa_total:.2f}")

# Fecha o ambiente e destrói a janela do SDL com segurança
env.close()
from stable_baselines3 import PPO
from asteroids_env import AsteroidsEnv
from pathlib import Path
from gymnasium.wrappers import FrameStackObservation

print("Carregando o ambiente com interface gráfica...")
env = AsteroidsEnv(
    render_mode="human",
    asteroidDestroyedReward = 1.0,
    loseReward = -1.0,
    winReward = 1.0,
    laserMissReward = 0.0,
)
env = FrameStackObservation(env, stack_size=4)

print("Carregando o modelo treinado...")

MODELS_DIR = Path("models")
path = MODELS_DIR / "CNN_2M_84px"
model = PPO.load(path)

obs, _ = env.reset()
done = False
total_reward = 0.0

print("Deixando a IA jogar...")

i = 0
while True:
    acao, _estados_internos = model.predict(obs, deterministic=True)

    obs, recompensa, done, trunc, _ = env.step(acao)

    total_reward += recompensa
    if done or trunc:
        print(f"\nFim de jogo {i}! Recompensa total da IA: {total_reward:.2f}")
        obs, _ = env.reset()
        total_reward = 0
        i+=1

# Fecha o ambiente e destrói a janela do SDL com segurança
env.close()
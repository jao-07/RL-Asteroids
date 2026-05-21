import optuna
from stable_baselines3 import DQN
from stable_baselines3.common.evaluation import evaluate_policy
from stable_baselines3.common.monitor import Monitor
from asteroids_env import AsteroidsEnv

def sample_dqn_params(trial):
    return {
        "learning_rate": trial.suggest_float("learning_rate", 1e-5, 1e-2, log=True),
        "batch_size": trial.suggest_categorical("batch_size", [64, 128, 256]),
        "exploration_fraction": trial.suggest_float("exploration_fraction", 0.1, 0.5),
        "target_update_interval": trial.suggest_categorical("target_update_interval", [1000, 5000, 10000]),
        "policy_kwargs": dict(net_arch=[
            trial.suggest_categorical("camada_1", [128, 256]),
            trial.suggest_categorical("camada_2", [128, 256])
        ])
    }

def objective(trial):
    kwargs = sample_dqn_params(trial)

    env = Monitor(AsteroidsEnv(render_mode="none"))
    model = DQN(
        "MlpPolicy",
        env,
        gamma=0.99,
        buffer_size=100000,
        learning_starts=1000,
        learning_rate: 3.254899305326533e-05
    batch_size: 64
    exploration_fraction: 0.21188211502601786
    target_update_interval: 1000
    camada_1: 128
    camada_2: 256

    verbose=0,
        **kwargs
    )

    try:
        model.learn(total_timesteps=150000)
    except Exception as e:
        return -1000.0

    mean_reward, _ = evaluate_policy(model, env, n_eval_episodes=10, deterministic=True)
    env.close()

    return mean_reward

if __name__ == "__main__":
    print("A iniciar a otimização de hiperparâmetros do Asteroids...")

    study = optuna.create_study(direction="maximize")
    study.optimize(objective, n_trials=30, n_jobs=1, show_progress_bar=True)

    print("\n=======================================================")
    print("🚀 MELHOR CONFIGURAÇÃO ENCONTRADA PELO OPTUNA:")
    print("=======================================================")
    print(f"Recompensa Máxima Média: {study.best_value:.2f}")
    print("Hiperparâmetros a usar no treino principal:")
    for key, value in study.best_trial.params.items():
        print(f"    {key}: {value}")
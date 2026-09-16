import numpy as np
import cv2
import asteroids_cpp

# Instancia o jogo
game = asteroids_cpp.Game(False, False, 10, 1.0, 0.5, 0.0, 1.0, 2.5, 0.1)
game.initialize()
game.reset()

# 1. Teste no Reset: Pega em 800x600
pixels_reset = game.get_screen_pixels()
img_reset = np.squeeze(pixels_reset)
cv2.imwrite("debug_reset_800x600.png", img_reset)

# 2. Executa alguns passos e captura novamente
for _ in range(5):
    game.step(1) # Executa uma ação qualquer

pixels_step = game.get_screen_pixels()
img_step = np.squeeze(pixels_step)
cv2.imwrite("debug_step_800x600.png", img_step)

print("Imagens salvas! Verifique 'debug_reset_800x600.png' e 'debug_step_800x600.png'")
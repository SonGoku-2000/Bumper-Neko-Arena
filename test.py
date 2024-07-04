
def determinar_direccion_giro(theta, theta_target):
    # theta = theta % 360
    # theta_target = theta_target % 360

    delta_theta = theta_target - theta
    print(delta_theta)
    delta_theta = (delta_theta + 180)
    print(delta_theta)
    delta_theta = (delta_theta  % 360)
    print(delta_theta)
    delta_theta = (delta_theta- 180)
    print(delta_theta)

    if delta_theta > 0:
        direccion = "izquierda"
    elif delta_theta < 0:
        direccion = "derecha"
    else:
        direccion = "ninguna"  # theta ya es igual a theta_target

    return direccion, abs(delta_theta)

theta_inicial = 342
theta_objetivo = 0

direccion, magnitud = determinar_direccion_giro(theta_inicial, theta_objetivo)
print(f"Debes girar a la {direccion} {magnitud} grados.")

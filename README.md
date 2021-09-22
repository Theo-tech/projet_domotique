<script>
    mermaid.initialize({
    sequence: { showSequenceNumbers: true },
    });
</script>
# Projet Domotique

## Contexte

Dans l'optique d'assimiler les concepts d'environnements intelligents et communiquants, un cas de test de type "domotique" a été retenu.   
L'objectif est dans mettre en oeuvre une version simple et allegée d'un système domotique domestique.  
Le système devra comprendre la lecture d'un capteur de température et d'hydrométrie pour ensuite commander l'asservissement d'une bouche d'aération que l'on pourrait retrouver dans une salle d'eau par exemple.  

## Schéma d'architecture

```mermaid
  graph LR
      Thermomètre/Hydromètre-->ESP32
      ESP32-->STM32
      ESP32-->Afficheur;
      STM32-->Servomoteur
      STM32-->Afficheur/Controleur
      Afficheur/Controleur-->STM32
      
```

## Schéma de Principe

```mermaid
sequenceDiagram
    participant Thermomètre/Hydromètre
    participant ESP32
    participant STM32
    participant Servomoteur
    autonumber
    Thermomètre/Hydromètre->>ESP32: Récupération de l'Advertiser
    loop
        ESP32->>ESP32: Extraction de la température et l'hydrométrie de l'advertiser
    end
    STM32->>Servomoteur: Demande de l'état actuel du servomoteur
    activate Servomoteur
    Servomoteur-->>STM32: Envoie son état actuel
    deactivate Servomoteur
    loop 
        STM32->>STM32: Prise de décision quand à l'asservissement du servomoteur
    end
    alt Si Servomoteur != position requise
        STM32->>Servomoteur: Commande le servomoteur
    end

```

## ESP32 : Récupération des valeurs

## STM32 : Asservissement du servomoteur
#!/bin/bash

# ═══════════════════════════════════════════════════
#              PHILO TESTER - 42 Project
# ═══════════════════════════════════════════════════

PHILO="./philo"
PASS=0
FAIL=0
TIMEOUT_INF=900  # 15 min pour les tests infinis

RED='\033[0;91m'
GREEN='\033[0;92m'
YELLOW='\033[0;93m'
CYAN='\033[0;96m'
RESET='\033[0m'

# ─────────────────────────────────────────────────────
# Helper: check si "died" apparait dans la sortie
# ─────────────────────────────────────────────────────
check_output_for_death()
{
    local output="$1"
    echo "$output" | grep -q "died"
}

# ─────────────────────────────────────────────────────
# Test qui NE DOIT PAS mourir (tourne X secondes)
# ─────────────────────────────────────────────────────
test_no_death()
{
    local args="$1"
    local duration="$2"
    local desc="$3"

    printf "${CYAN}[TEST - NO DEATH]${RESET} $desc | args: $args | durée: ${duration}s\n"
    output=$(timeout $duration $PHILO $args 2>&1)
    exit_code=$?

    if check_output_for_death "$output"; then
        printf "${RED}  ✗ FAIL${RESET} - Un philo est mort alors qu'il ne devrait pas !\n"
        echo "$output" | grep "died"
        FAIL=$((FAIL + 1))
    elif [ $exit_code -eq 124 ]; then
        printf "${GREEN}  ✓ PASS${RESET} - Personne n'est mort en $duration secondes.\n"
        PASS=$((PASS + 1))
    else
        printf "${YELLOW}  ? WARN${RESET} - Le programme s'est arrêté tout seul (exit $exit_code). Vérifie.\n"
        FAIL=$((FAIL + 1))
    fi
    echo ""
}

# ─────────────────────────────────────────────────────
# Test qui DOIT mourir (dans un délai raisonnable)
# ─────────────────────────────────────────────────────
test_must_die()
{
    local args="$1"
    local max_wait="$2"
    local desc="$3"

    printf "${CYAN}[TEST - MUST DIE]${RESET} $desc | args: $args | max: ${max_wait}s\n"
    output=$(timeout $max_wait $PHILO $args 2>&1)
    exit_code=$?

    if check_output_for_death "$output"; then
        # Vérifier qu'aucun message n'apparait APRES "died"
        after=$(echo "$output" | awk '/died/{found=1; next} found{print}')
        if [ -n "$after" ]; then
            printf "${RED}  ✗ FAIL${RESET} - Messages après la mort !\n"
            echo "$after"
            FAIL=$((FAIL + 1))
        else
            printf "${GREEN}  ✓ PASS${RESET} - Mort détectée correctement, aucun message après.\n"
            PASS=$((PASS + 1))
        fi
    else
        printf "${RED}  ✗ FAIL${RESET} - Personne n'est mort alors qu'il le devrait !\n"
        FAIL=$((FAIL + 1))
    fi
    echo ""
}

# ─────────────────────────────────────────────────────
# Test max_eat (doit s'arrêter tout seul)
# ─────────────────────────────────────────────────────
test_max_eat()
{
    local args="$1"
    local max_wait="$2"
    local desc="$3"

    printf "${CYAN}[TEST - MAX EAT]${RESET} $desc | args: $args | max: ${max_wait}s\n"
    output=$(timeout $max_wait $PHILO $args 2>&1)
    exit_code=$?

    if check_output_for_death "$output"; then
        printf "${RED}  ✗ FAIL${RESET} - Quelqu'un est mort avec max_eat !\n"
        FAIL=$((FAIL + 1))
    elif [ $exit_code -eq 0 ]; then
        printf "${GREEN}  ✓ PASS${RESET} - Programme terminé proprement avec max_eat.\n"
        PASS=$((PASS + 1))
    elif [ $exit_code -eq 124 ]; then
        printf "${RED}  ✗ FAIL${RESET} - Timeout ! Le programme ne s'est pas arrêté avec max_eat.\n"
        FAIL=$((FAIL + 1))
    else
        printf "${YELLOW}  ? WARN${RESET} - Exit inattendu ($exit_code).\n"
        FAIL=$((FAIL + 1))
    fi
    echo ""
}

# ═══════════════════════════════════════════════════
#                    DEBUT DES TESTS
# ═══════════════════════════════════════════════════

echo ""
printf "${YELLOW}════════════════════════════════════════════${RESET}\n"
printf "${YELLOW}        PHILO TESTER - START                ${RESET}\n"
printf "${YELLOW}════════════════════════════════════════════${RESET}\n"
echo ""

# ─────────────────────────────────────────────────────
# TESTS DOIT PAS MOURIR (courts - 10s)
# ─────────────────────────────────────────────────────
printf "${YELLOW}--- Tests rapides (10s) - personne ne doit mourir ---${RESET}\n\n"

test_no_death "4 800 200 200"   10 "4 philos basique"
test_no_death "2 800 200 200"   10 "2 philos"
test_no_death "6 800 200 200"   10 "6 philos"
test_no_death "8 800 200 200"   10 "8 philos"
test_no_death "10 1000 200 200" 10 "10 philos"
test_no_death "5 2000 300 300"  10 "5 philos large time_to_die"
test_no_death "3 1210 400 400"  10 "3 philos"
test_no_death "7 900 250 250"   10 "7 philos"
test_no_death "4 2000 200 200"  10 "4 philos confort"
test_no_death "6 1000 300 200"  10 "6 philos"
test_no_death "20 800 200 200"  10 "20 philos"
test_no_death "50 800 200 200"  10 "50 philos"
test_no_death "100 800 200 200" 10 "100 philos stress"
test_no_death "200 800 200 200" 10 "200 philos gros stress"

# ─────────────────────────────────────────────────────
# TESTS DOIT MOURIR
# ─────────────────────────────────────────────────────
printf "${YELLOW}--- Tests - quelqu'un doit mourir ---${RESET}\n\n"

test_must_die "4 310 200 100"  5  "4 philos mort rapide"
test_must_die "2 500 300 300"  5  "2 philos borderline"
test_must_die "5 300 150 150"  5  "5 philos mort rapide"
test_must_die "3 400 200 300"  5  "3 philos - time_to_sleep > time_to_die"
test_must_die "4 200 100 100"  5  "4 philos - très serré"
test_must_die "10 200 100 100" 5  "10 philos mort rapide"

# ─────────────────────────────────────────────────────
# TESTS MAX EAT
# ─────────────────────────────────────────────────────
printf "${YELLOW}--- Tests max_eat - doit s'arrêter proprement ---${RESET}\n\n"

test_max_eat "4 800 200 200 3"  20 "4 philos 3 repas chacun"
test_max_eat "5 800 200 200 5"  30 "5 philos 5 repas chacun"
test_max_eat "2 800 200 200 7"  30 "2 philos 7 repas chacun"
test_max_eat "6 800 200 200 2"  20 "6 philos 2 repas chacun"
test_max_eat "4 800 200 200 10" 60 "4 philos 10 repas chacun"

# ─────────────────────────────────────────────────────
# TESTS LONGUE DUREE (15 min) - personne ne doit mourir
# ─────────────────────────────────────────────────────
printf "${YELLOW}--- Tests longue durée (15 min) - personne ne doit mourir ---${RESET}\n\n"
printf "${RED}⚠ Ces tests durent 15 min chacun, vas te faire un café ☕${RESET}\n\n"

test_no_death "4 800 200 200"   $TIMEOUT_INF "4 philos - 15 min"
test_no_death "5 800 200 200"   $TIMEOUT_INF "5 philos - 15 min"
test_no_death "6 1000 200 200"  $TIMEOUT_INF "6 philos - 15 min"
test_no_death "10 800 200 200"  $TIMEOUT_INF "10 philos - 15 min"
test_no_death "4 2000 200 200"  $TIMEOUT_INF "4 philos large - 15 min"

# ═══════════════════════════════════════════════════
#                    RESULTATS
# ═══════════════════════════════════════════════════
echo ""
printf "${YELLOW}════════════════════════════════════════════${RESET}\n"
printf "${YELLOW}                 RESULTATS                  ${RESET}\n"
printf "${YELLOW}════════════════════════════════════════════${RESET}\n"
printf "${GREEN}  PASS : $PASS${RESET}\n"
printf "${RED}  FAIL : $FAIL${RESET}\n"
TOTAL=$((PASS + FAIL))
printf "  TOTAL: $TOTAL\n"
printf "${YELLOW}════════════════════════════════════════════${RESET}\n"
echo ""
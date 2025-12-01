
#!/bin/bash
# ==========================================
# Analyse-Skript, das im Ordner "script/" liegt
# und automatisch das Projekt-Hauptverzeichnis findet.
# source .venv/bin/activate
# ==========================================

# Absoluter Pfad zu diesem Script
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

# Projekt-Hauptverzeichnis (eine Ebene darüber)
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

echo "📁 Script-Verzeichnis: $SCRIPT_DIR"
echo "📁 Projekt-Wurzel:     $PROJECT_ROOT"

# virtuelle Umgebung aktivieren
if [ -d "$PROJECT_ROOT/.venv" ]; then
    source "$PROJECT_ROOT/.venv/bin/activate"
else
    echo "❌ Keine virtuelle Umgebung gefunden unter:"
    echo "   $PROJECT_ROOT/.venv"
    exit 1
fi

# Ordner, die analysiert werden sollen
outputs=(
    "$PROJECT_ROOT/results/nov25/output_simpleFingerTest"
)

# Schleife über alle Output-Ordner
for output in "${outputs[@]}"; do
    if [ ! -d "$output" ]; then
        echo "❌ Ordner existiert nicht: $output"
        continue
    fi

    model_name="$(basename "$output")"
    model_name="${model_name#output_}"
    echo "===> 🧠 Analysiere: $model_name"

    cd "$output" || exit 1

    # Python-Skripte im Script-Ordner ausführen
    python3 "$SCRIPT_DIR/plot_momenarm.py" "$model_name"
    python3 "$SCRIPT_DIR/plot.py" -model "$model_name" -scale 0.09 -viewangle -90

    cd - > /dev/null
    echo "✅ Fertig mit: $model_name"
done

echo "🎉 Alle Analysen abgeschlossen!"

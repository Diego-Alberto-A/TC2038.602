from flask import Flask, request, jsonify, send_from_directory
import csv
import os

app = Flask(__name__, static_folder='.')
CSV_FILE = 'ranking.csv'


@app.route('/')
def index():
    return send_from_directory('.', 'index.html')


@app.route('/<path:path>')
def serve_static(path):
    return send_from_directory('.', path)


@app.route('/api/ranking', methods=['GET'])
def get_ranking():
    if not os.path.exists(CSV_FILE):
        return jsonify([])

    data = []
    with open(CSV_FILE, mode='r', encoding='utf-8') as f:
        reader = csv.reader(f)
        for row in reader:
            if len(row) == 2:
                try:
                    data.append({'name': row[0].strip(), 'score': int(row[1].strip())})
                except ValueError:
                    pass
    return jsonify(data)


@app.route('/api/ranking', methods=['POST'])
def save_ranking():
    data = request.json
    with open(CSV_FILE, mode='w', newline='', encoding='utf-8') as f:
        writer = csv.writer(f)
        for item in data:
            writer.writerow([item.get('name'), item.get('score')])
    return jsonify({'status': 'success'})


if __name__ == '__main__':
    app.run(debug=True, port=5000)

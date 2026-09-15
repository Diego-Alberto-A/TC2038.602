const STORAGE_KEY = 'ranking.csv';

const inputNombre = document.getElementById('inputNombre');
const textoError = document.getElementById('textoError');
const btnBuscar = document.getElementById('btnBuscar');
const btnVolver = document.getElementById('btnVolver');
const pantallaBusqueda = document.getElementById('pantallaBusqueda');
const pantallaResultados = document.getElementById('pantallaResultados');

btnBuscar.addEventListener('click', handleSearch);
btnVolver.addEventListener('click', () => toggleScreens(false));

function getLatestData() {
  const savedData = localStorage.getItem(STORAGE_KEY);
  if (!savedData) return [];

  const lines = savedData.trim().split('\n');
  return lines.map(line => {
    const [name, score] = line.split(',');
    return { name: name.trim(), score: parseInt(score, 10) };
  }).filter(player => player.name !== '' && !isNaN(player.score));
}

function handleSearch() {
  const currentParticipants = getLatestData();

  if (currentParticipants.length === 0) {
    return showError('No hay datos en el archivo ranking.csv');
  }

  const searchQuery = inputNombre.value.toLowerCase().trim();
  const targetIndex = currentParticipants.findIndex(
    (player) => player.name.toLowerCase() === searchQuery
  );

  if (targetIndex === -1) {
    return showError('Nombre no encontrado');
  }

  clearError();
  const finalPivotIndex = applyLomutoPartition(currentParticipants, targetIndex);
  renderResults(currentParticipants, finalPivotIndex);
  toggleScreens(true);
}

function applyLomutoPartition(array, pivotIndex) {
  const lastIndex = array.length - 1;
  const tempPlayer = array[pivotIndex];
  array[pivotIndex] = array[lastIndex];
  array[lastIndex] = tempPlayer;

  const pivotScore = array[lastIndex].score;
  let insertIndex = 0;

  for (let currentIndex = 0; currentIndex < lastIndex; currentIndex++) {
    if (array[currentIndex].score > pivotScore) {
      const swapPlayer = array[insertIndex];
      array[insertIndex] = array[currentIndex];
      array[currentIndex] = swapPlayer;
      insertIndex++;
    }
  }

  const finalSwapPlayer = array[insertIndex];
  array[insertIndex] = array[lastIndex];
  array[lastIndex] = finalSwapPlayer;

  return insertIndex;
}

function renderResults(array, pivotIndex) {
  const targetPlayer = array[pivotIndex];
  const totalPlayers = array.length;

  document.getElementById('resNombre').innerText = targetPlayer.name;
  document.getElementById('resScore').innerText = targetPlayer.score;
  document.getElementById('resRank').innerText = `#${pivotIndex + 1}`;
  document.getElementById('resDelante').innerText = pivotIndex;
  document.getElementById('resDetras').innerText = totalPlayers - 1 - pivotIndex;

  const listAhead = array.slice(0, pivotIndex);
  const listBehind = array.slice(pivotIndex + 1);

  document.getElementById('listaDelante').innerHTML = generateListHTML(listAhead);
  document.getElementById('marcadorNombre').innerText = targetPlayer.name;
  document.getElementById('listaDetras').innerHTML = generateListHTML(listBehind);
}

function generateListHTML(playerArray) {
  if (playerArray.length === 0) return '<li>- Ninguno -</li>';
  return playerArray.map((player) => `<li>${player.name}</li>`).join('');
}

function showError(message) {
  textoError.innerText = message;
}

function clearError() {
  textoError.innerText = '';
}

function toggleScreens(showResultsScreen) {
  pantallaBusqueda.hidden = showResultsScreen;
  pantallaResultados.hidden = !showResultsScreen;
}

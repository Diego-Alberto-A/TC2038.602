let participantsData = [];

const inputNuevoNombre = document.getElementById('inputNuevoNombre');
const inputNuevoScore = document.getElementById('inputNuevoScore');
const btnAgregar = document.getElementById('btnAgregar');
const btnGuardar = document.getElementById('btnGuardar');
const listaManage = document.getElementById('listaManage');
const textoErrorManage = document.getElementById('textoErrorManage');
const textoSuccess = document.getElementById('textoSuccess');

window.addEventListener('DOMContentLoaded', loadInitialData);
btnAgregar.addEventListener('click', addRecord);
btnGuardar.addEventListener('click', saveModifiedData);

async function loadInitialData() {
  const response = await fetch('/api/ranking');
  participantsData = await response.json();
  renderManageList();
}

function renderManageList() {
  listaManage.innerHTML = '';

  if (participantsData.length === 0) {
    listaManage.innerHTML = '<li>- Sin datos -</li>';
    return;
  }

  participantsData.forEach((player, index) => {
    const li = document.createElement('li');
    li.className = 'manage-item';

    const span = document.createElement('span');
    span.innerText = `${player.name} (${player.score})`;

    const deleteBtn = document.createElement('button');
    deleteBtn.innerText = 'Eliminar';
    deleteBtn.className = 'btn-delete';
    deleteBtn.addEventListener('click', () => deleteRecord(index));

    li.appendChild(span);
    li.appendChild(deleteBtn);
    listaManage.appendChild(li);
  });
}

function addRecord() {
  const name = inputNuevoNombre.value.trim();
  const score = parseInt(inputNuevoScore.value, 10);

  if (!name || isNaN(score)) {
    textoErrorManage.innerText = 'Ingrese nombre y score válido';
    return;
  }

  participantsData.push({ name, score });
  inputNuevoNombre.value = '';
  inputNuevoScore.value = '';
  textoErrorManage.innerText = '';
  renderManageList();
}

function deleteRecord(index) {
  participantsData.splice(index, 1);
  renderManageList();
}

async function saveModifiedData() {
  await fetch('/api/ranking', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(participantsData),
  });

  textoSuccess.innerText = 'Ranking actualizado!';
  textoSuccess.style.display = 'block';
  
}

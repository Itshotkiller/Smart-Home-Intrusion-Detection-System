const express = require('express');
const admin = require('firebase-admin');
const serviceAccount = require('./serviceAccountKey.json');

const app = express();
app.use(express.json());

admin.initializeApp({
  credential: admin.credential.cert(serviceAccount),
  databaseURL: "https://smarthomesecurity-9330d-default-rtdb.firebaseio.com"
});

const db = admin.database();

app.post('/alert', (req, res) => {
  const { type, message } = req.body;
  if (!type || !message) {
    return res.status(400).send('Invalid request');
  }

  const alert = {
    type,
    message,
    timestamp: new Date().toISOString()
  };

  db.ref('alerts').push(alert)
    .then(() => res.status(200).send('Alert saved'))
    .catch((err) => res.status(500).send('Error saving alert'));
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
  console.log(`Server running on port ${PORT}`);
});
import './App.css';
import React from 'react';

import { BrowserRouter, Route, Routes } from 'react-router-dom';

// Pages
import Control from './pages/control/Control';
import Login from './pages/login/Login';
import Waiting from './pages/waiting/Waiting';

function App() {
  return (
    <div className='app-container'>
      <BrowserRouter>
        <Routes>
          <Route path="*" element={<Control />} />
          <Route path="login" element={<Login />} />
          <Route path="waiting" element={<Waiting />} />
        </Routes>
      </BrowserRouter>
    </div>
  );
}

export default App;

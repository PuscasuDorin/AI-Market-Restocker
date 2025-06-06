// Untitled-3.js

document.addEventListener('DOMContentLoaded', () => {
  const cartIcon      = document.getElementById('cart-icon');
  const cartPopup     = document.getElementById('cart-popup');
  const cartItemsList = document.getElementById('cart-items');
  const cartTotalDiv  = document.getElementById('cart-total');
  const checkoutBtn   = document.getElementById('checkout-btn');

  // Obiect care reține produsele adăugate în coș:
  // { '1': { name: 'Produs 1', price: 20, quantity: 2 }, ... }
  let cart = {};

  // Funcție pentru afișarea/ascunderea pop-up-ului coșului
  cartIcon.addEventListener('click', () => {
    cartPopup.style.display = (cartPopup.style.display === 'block') ? 'none' : 'block';
  });

  // Ascunde coșul dacă se dă click în afara lui
  document.addEventListener('click', (e) => {
    if (!cartIcon.contains(e.target) && !cartPopup.contains(e.target)) {
      cartPopup.style.display = 'none';
    }
  });

  // Funcție care actualizează lista de articole și totalul din coș
  function updateCartDisplay() {
    cartItemsList.innerHTML = '';  // golește lista curentă

    let totalSum = 0;
    for (let id in cart) {
      const item = cart[id];
      const subTotal = item.price * item.quantity;

      // Construim <li> cu text + buton de ștergere
      const li = document.createElement('li');
      li.dataset.id = id;

      // Textul articolului (ex: "Produs 1 x 2 → 40 RON")
      const textNode = document.createTextNode(`${item.name} x ${item.quantity} → ${subTotal} RON`);
      li.appendChild(textNode);

      // Butonul de ștergere (×)
      const removeBtn = document.createElement('button');
      removeBtn.textContent = '×';
      removeBtn.title = 'Șterge un produs';
      removeBtn.classList.add('remove-from-cart');
      removeBtn.dataset.id = id;
      li.appendChild(removeBtn);

      cartItemsList.appendChild(li);
      totalSum += subTotal;
    }

    cartTotalDiv.textContent = `Total: ${totalSum} RON`;

    // După ce am reconstruit lista, atașăm evenimente pentru butoanele de ștergere
    const allRemoveBtns = cartItemsList.querySelectorAll('.remove-from-cart');
    allRemoveBtns.forEach(btn => {
      btn.addEventListener('click', handleRemoveFromCart);
    });
  }

  // Funcție care se ocupă de adăugarea în coș
  function handleAddToCart(e) {
    const productDiv = e.currentTarget.closest('.product-item');
    const id         = productDiv.getAttribute('data-id');
    const name       = productDiv.getAttribute('data-name');
    const price      = parseFloat(productDiv.getAttribute('data-price'));
    const stockSpan  = document.getElementById(`stock-${id}`);

    // Inițializează produsul în cart dacă nu există
    if(parseInt(stockSpan.textContent) > 0 ){
      if (!cart[id]) {
        cart[id] = {
          name: name,
          price: price,
          quantity: 0
        };
      }

      // Incrementăm cantitatea în coș
      cart[id].quantity += 1;

      // Actualizăm și stocul din pagina principală (crescut cu 1)
      let currentStock = parseInt(stockSpan.textContent, 10);
      currentStock -= 1;
      stockSpan.textContent = currentStock;

      // Re-afișăm coșul și totalul
      updateCartDisplay();
    }
  }

  // Funcție care se ocupă de ștergerea unui singur produs din coș
  function handleRemoveFromCart(e) {
    e.stopPropagation(); // prevenim propagarea click-ului către <li> sau alte elemente
    const id = e.currentTarget.dataset.id;
    const stockSpan = document.getElementById(`stock-${id}`);

    // Dacă produsul există în cart și cantitatea > 0
    if (cart[id] && cart[id].quantity > 0) {
      // Decrementăm cantitatea în cart
      cart[id].quantity -= 1;

      // Decrementăm și afișajul stocului (scade cu 1)
        let currentStock = parseInt(stockSpan.textContent, 10);
        currentStock = Math.max(0, currentStock + 1);
        stockSpan.textContent = currentStock;
      // Dacă după decrementare cantitatea din cart e 0, îl eliminăm complet
      if (cart[id].quantity === 0) {
        delete cart[id];
      }

      // Re-afișăm coșul și totalul
      updateCartDisplay();
    }
  }

  // Funcție care golește întreg coșul și resetează stocurile la 0
  function handleCheckout() {
    // Pentru fiecare produs din cart, resetăm stocul afișat la zero
    for (let id in cart) {
      const stockSpan = document.getElementById(`stock-${id}`);
    }
    // Golește obiectul cart
    cart = {};
    // Re-afișează lista (care va fi acum goală) și totalul (0)
    updateCartDisplay();

    // (Opțional poți arăta un mesaj că „comanda a fost plasată”)
    alert('Comanda a fost plasată cu succes!');
  }

  // Atașăm evenimentele de click pentru butoanele „Adaugă în coș”
  const addButtons = document.querySelectorAll('.add-to-cart');
  addButtons.forEach(btn => {
    btn.addEventListener('click', handleAddToCart);
  });

  // Atașăm evenimentul butonului „Dă comanda”
  checkoutBtn.addEventListener('click', handleCheckout);
});

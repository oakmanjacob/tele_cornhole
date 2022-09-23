export const controlKeyboardListener = (keyboardEvent: KeyboardEvent) => {

    // For now, let's just handle key presses
    if (keyboardEvent.type !== 'keypress') {
        return;
    }
    console.log(keyboardEvent)
    switch (keyboardEvent.code) {
        case 'Space':
            console.log('Space');

    }
}
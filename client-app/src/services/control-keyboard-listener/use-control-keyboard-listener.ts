import { useEffect } from "react";
import { controlKeyboardListener } from "./control-keyboard-listener";

export const useControlKeyboardListener = () => {
    useEffect(() => {
        window.addEventListener('keypress', controlKeyboardListener);
        return () => {
            window.removeEventListener('keypress', controlKeyboardListener);
        }
    })
}
import './login-form.css';

export const LoginForm: React.FC = () => {
    return (
        <form className="login-form-container">
            <input placeholder="name" />
            <input placeholder="super secret code" />
            <button type="submit">Submit</button>
        </form>
    )
}
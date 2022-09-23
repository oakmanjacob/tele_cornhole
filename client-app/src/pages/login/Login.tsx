import { LoginForm } from '../../components/login-form/login-form'
import { WelcomeHeading } from '../../components/welcome-heading/welcome-heading'
import './Login.css'

const Login: React.FC = () => {
    return (
        <div className="page-container login-container">
            <WelcomeHeading />
            <LoginForm />
        </div>
    )
}

export default Login
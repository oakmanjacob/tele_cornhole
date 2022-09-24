import { FeedRenderer } from '../../components/feed-renderer/feed-renderer'
import { LoginForm } from '../../components/login-form/login-form'
import { WelcomeHeading } from '../../components/welcome-heading/welcome-heading'
import './Login.css'

const Login: React.FC = () => {
    return (
        <div className="page-container login-container">
            {/* <WelcomeHeading /> */}
            <FeedRenderer />
            {/* <LoginForm /> */}
        </div>
    )
}

export default Login
import "./App.css";
import React, { Component } from "react";
import axios from "axios"; //npm install axios
class App extends React.Component {
  constructor() {
    super();
    this.state = {
      items: [],
    };
  }
  componentDidMount() {
    axios.get("http://localhost:5555/api/sensors").then((response) => {
      console.log(response.data);
      const data = response.data;
      this.setState({ items: data });
    });
  }
  render() {
    return (
      <div className="App">
        <h1>List devices</h1>
        <ul>
          {this.state.items.map((item) => (
            <li key={item.id}>
              id: {item.id}, name: {item.name}, temperature: {item.temperature}, humide: {item.humid}
            </li>
          ))}
        </ul>
      </div>
    );
  }
}
export default App;
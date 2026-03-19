---
name: create-pr-on-stop
enabled: true
event: stop
action: warn
---

A PR has not been created yet for this session's changes.

Before finishing, please create a pull request using the `/pr` skill:
- Run the `/pr` skill to open a PR for the current branch against `master`.
- GitHub credentials are in `~/.githubcredentials`.
- The PR title should summarize the work done in this session.

If a PR was already created, you can ignore this reminder.
